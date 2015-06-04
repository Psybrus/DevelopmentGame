/**************************************************************************
*
* File:		GaTestShaderComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestShaderComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// GaVertex
struct GaVertex
{
	GaVertex()
	{}

	GaVertex( MaVec4d Position, MaVec4d Normal, MaVec4d Tangent, MaVec4d Colour, MaVec2d TexCoord ):
		Position_( Position ),
		Normal_( Normal ),
		Tangent_( Tangent ),
		Colour_( Colour ),
		TexCoord_( TexCoord )
	{}

	MaVec4d Position_;
	MaVec4d Normal_;
	MaVec4d Tangent_;
	MaVec4d Colour_;
	MaVec2d TexCoord_;
};

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestShaderComponent );

void GaTestShaderComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_", &GaTestShaderComponent::Material_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "ObjectUniformBuffer_", &GaTestShaderComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaTestShaderComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaTestShaderComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaTestShaderComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaTestShaderComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent_", &GaTestShaderComponent::MaterialComponent_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaTestShaderComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestShaderComponent::GaTestShaderComponent():
	ObjectUniformBuffer_( nullptr ),
	TestUniformBuffer_( nullptr ),
	IndexBuffer_( nullptr ),
	VertexBuffer_( nullptr ),
	VertexDeclaration_( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestShaderComponent::~GaTestShaderComponent()
{
}

//////////////////////////////////////////////////////////////////////////
// render
class GaTestShaderComponentRenderNode: public RsRenderNode
{
public:
	void render( RsContext* Context )
	{
		PSY_PROFILER_SECTION( RenderRoot, "GaTestShaderComponentRenderNode::render" );
		Context->setVertexBuffer( 0, VertexBuffer_, sizeof( GaVertex ) );
		Context->setVertexDeclaration( VertexDeclaration_ );
		Context->drawPrimitives( Type_, Offset_, NoofIndices_ );
	}

	RsTopologyType Type_;
	BcU32 Offset_;
	BcU32 NoofIndices_;
	RsBuffer* VertexBuffer_;
	RsVertexDeclaration* VertexDeclaration_;
};


//virtual 
void GaTestShaderComponent::render( class ScnViewComponent* pViewComponent, RsFrame* pFrame, RsRenderSort Sort )
{
	Super::render( pViewComponent, pFrame, Sort );

	RsCore::pImpl()->updateBuffer( 
		ObjectUniformBuffer_,
		0, sizeof( ScnShaderObjectUniformBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
			UniformBlock->WorldTransform_ = MaMat4d();
		} );


	// Set skinning parameters.
	MaterialComponent_->setObjectUniformBlock( ObjectUniformBuffer_ );
			
	// Set material components for view.
	pViewComponent->setMaterialParameters( MaterialComponent_ );
			
	// Bind material.
	MaterialComponent_->bind( pFrame, Sort );


	// Render primitive.
	GaTestShaderComponentRenderNode* pRenderNode = pFrame->newObject< GaTestShaderComponentRenderNode >();
			
	pRenderNode->Type_ = RsTopologyType::TRIANGLE_STRIP;
	pRenderNode->Offset_ = 0;
	pRenderNode->NoofIndices_ = 4;
	pRenderNode->VertexBuffer_ = VertexBuffer_;
	pRenderNode->VertexDeclaration_ = VertexDeclaration_;
	pRenderNode->Sort_ = Sort;
			
	pFrame->addRenderNode( pRenderNode );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestShaderComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ) );

	BcU32 IndexBufferSize = sizeof( BcU16 ) * 4;
	IndexBuffer_ = RsCore::pImpl()->createBuffer(
		RsBufferDesc( RsBufferType::INDEX, RsResourceCreationFlags::STATIC, IndexBufferSize ) );

	RsCore::pImpl()->updateBuffer( 
		IndexBuffer_, 0, IndexBufferSize, RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& BufferLock )
		{
			BcU16* Indices = reinterpret_cast< BcU16* >( BufferLock.Buffer_ );
			Indices[ 0 ] = 0;
			Indices[ 1 ] = 1;
			Indices[ 2 ] = 2;
			Indices[ 3 ] = 3;
		} );

	BcU32 VertexBufferSize = 2048 * sizeof( GaVertex );
	VertexBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsBufferType::VERTEX,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ) );

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 2, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ) );

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_,
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
		} );

	ScnShaderPermutationFlags ShaderPermutation = 
		ScnShaderPermutationFlags::MESH_STATIC_3D |
		ScnShaderPermutationFlags::RENDER_FORWARD |
		ScnShaderPermutationFlags::LIGHTING_NONE;

	// Attach a new material component.
	MaterialComponent_ = Parent->attach< ScnMaterialComponent >(
		BcName::INVALID, Material_, ShaderPermutation );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestShaderComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	Parent->detach( MaterialComponent_ );
	MaterialComponent_ = nullptr;

	RsCore::pImpl()->destroyResource( VertexDeclaration_ );
	RsCore::pImpl()->destroyResource( VertexBuffer_ );
	RsCore::pImpl()->destroyResource( IndexBuffer_ );
	RsCore::pImpl()->destroyResource( ObjectUniformBuffer_ );
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestShaderComponent::getAABB() const
{
	return MaAABB();
}
