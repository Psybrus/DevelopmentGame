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

#include "System/Scene/ScnShaderFileData.h"
#include "System/Scene/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// GaVertex
struct GaVertex
{
	GaVertex()
	{}

	GaVertex( MaVec3d Position, MaVec3d Normal, MaVec3d Tangent, MaVec4d Colour, MaVec2d TexCoord ):
		Position_( Position ),
		Normal_( Normal ),
		Tangent_( Tangent ),
		Colour_( Colour ),
		TexCoord_( TexCoord )
	{}

	MaVec3d Position_;
	MaVec3d Normal_;
	MaVec3d Tangent_;
	MaVec4d Colour_;
	MaVec2d TexCoord_;
};

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
DEFINE_RESOURCE( GaTestShaderComponent );

void GaTestShaderComponent::StaticRegisterClass()
{
	ReRegisterClass< GaTestShaderComponent, Super >()
		.addAttribute( new ScnComponentAttribute( 0 ) );
}

//////////////////////////////////////////////////////////////////////////
// initialise
void GaTestShaderComponent::initialise( const Json::Value& Object )
{
	Super::initialise( Object );

	Material_ = this->getPackage()->getPackageCrossRef( Object[ "material" ].asUInt() );

	BcU32 ShaderPermutation = 
		ScnShaderPermutationFlags::scnSPF_MESH_STATIC_3D |
		ScnShaderPermutationFlags::scnSPF_RENDER_FORWARD |
		ScnShaderPermutationFlags::scnSPF_LIGHTING_NONE;

	CsCore::pImpl()->createResource( BcName::INVALID, getPackage(), MaterialComponent_, Material_, ShaderPermutation );

}

//////////////////////////////////////////////////////////////////////////
// update
//virtual
void GaTestShaderComponent::update( BcF32 Tick )
{
	Super::update( Tick );
}

//////////////////////////////////////////////////////////////////////////
// render
class GaTestShaderComponentRenderNode: public RsRenderNode
{
public:
	void render()
	{
		PSY_PROFILER_SECTION( RenderRoot, "GaTestShaderComponentRenderNode::render" );
		pContext_->setVertexBuffer( 0, VertexBuffer_ );
		pContext_->setVertexDeclaration( VertexDeclaration_ );
		pContext_->drawPrimitives( Type_, Offset_, NoofIndices_ );
	}

	RsPrimitiveType Type_;
	BcU32 Offset_;
	BcU32 NoofIndices_;
	RsVertexBuffer* VertexBuffer_;
	RsVertexDeclaration* VertexDeclaration_;
};


//virtual 
void GaTestShaderComponent::render( class ScnViewComponent* pViewComponent, RsFrame* pFrame, RsRenderSort Sort )
{
	Super::render( pViewComponent, pFrame, Sort );

	auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( ObjectUniformBuffer_->lock() );
	UniformBlock->WorldTransform_ = MaMat4d();
	ObjectUniformBuffer_->unlock();

	// Set skinning parameters.
	MaterialComponent_->setObjectUniformBlock( ObjectUniformBuffer_ );
			
	// Set material components for view.
	pViewComponent->setMaterialParameters( MaterialComponent_ );
			
	// Bind material.
	MaterialComponent_->bind( pFrame, Sort );


	// Render primitive.
	GaTestShaderComponentRenderNode* pRenderNode = pFrame->newObject< GaTestShaderComponentRenderNode >();
			
	pRenderNode->Type_ = RsPrimitiveType::TRIANGLESTRIP;
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

	BcU16 Indices[4] = 
	{
		0, 1, 2, 3,
	};

	ObjectUniformBuffer_ = RsCore::pImpl()->createUniformBuffer( RsUniformBufferDesc( sizeof( ScnShaderObjectUniformBlockData ) ) );
	IndexBuffer_ = RsCore::pImpl()->createIndexBuffer( RsIndexBufferDesc( 4 ), Indices );
	VertexBuffer_ = RsCore::pImpl()->createVertexBuffer( RsVertexBufferDesc( 2048, sizeof( GaVertex ) ) );
	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 4 )
		.addElement( RsVertexElement( 0,  0, 3, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 3, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 3, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 52, 2, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ) );

	auto Vertices = reinterpret_cast< GaVertex* >( VertexBuffer_->lock() );
	*Vertices++ = GaVertex( MaVec3d( -1.0f, -1.0f,  0.0f ) * 10.0f, MaVec3d( 0.0f, 0.0f, 1.0f ), MaVec3d( 1.0f, 0.0f, 0.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
	*Vertices++ = GaVertex( MaVec3d(  1.0f, -1.0f,  0.0f ) * 10.0f, MaVec3d( 0.0f, 0.0f, 1.0f ), MaVec3d( 1.0f, 0.0f, 0.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
	*Vertices++ = GaVertex( MaVec3d( -1.0f,  1.0f,  0.0f ) * 10.0f, MaVec3d( 0.0f, 0.0f, 1.0f ), MaVec3d( 1.0f, 0.0f, 0.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
	*Vertices++ = GaVertex( MaVec3d(  1.0f,  1.0f,  0.0f ) * 10.0f, MaVec3d( 0.0f, 0.0f, 1.0f ), MaVec3d( 1.0f, 0.0f, 0.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
	VertexBuffer_->unlock();

	Parent->attach( MaterialComponent_ );

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
