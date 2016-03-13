/**************************************************************************
*
* File:		GaFullscreenQuadComponent .cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaFullscreenQuadComponent.h"

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
REFLECTION_DEFINE_DERIVED( GaFullscreenQuadComponent );

void GaFullscreenQuadComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_", &GaFullscreenQuadComponent::Material_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "MaterialComponent_", &GaFullscreenQuadComponent::MaterialComponent_, bcRFF_TRANSIENT ),
		new ReField( "ObjectUniformBuffer_", &GaFullscreenQuadComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaFullscreenQuadComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaFullscreenQuadComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaFullscreenQuadComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaFullscreenQuadComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaFullscreenQuadComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaFullscreenQuadComponent::GaFullscreenQuadComponent():
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
GaFullscreenQuadComponent::~GaFullscreenQuadComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// render
//virtual 
void GaFullscreenQuadComponent::render( ScnRenderContext & RenderContext )
{
	Super::render( RenderContext );

	RsCore::pImpl()->updateBuffer( 
		ObjectUniformBuffer_.get(),
		0, sizeof( ScnShaderObjectUniformBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
			UniformBlock->WorldTransform_ = MaMat4d();
		} );


	// Set skinning parameters.
	MaterialComponent_->setObjectUniformBlock( ObjectUniformBuffer_.get() );
			
	// Set material components for view.
	RenderContext.pViewComponent_->setMaterialParameters( MaterialComponent_ );
	
	// Render primitive.
	RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
		[ 
			GeometryBinding = GeometryBinding_.get(),
			ProgramBinding = MaterialComponent_->getProgramBinding(),
			RenderState = MaterialComponent_->getRenderState(),
			FrameBuffer = RenderContext.pViewComponent_->getFrameBuffer(),
			Viewport = RenderContext.pViewComponent_->getViewport()
		]
		( RsContext* Context )
		{
			PSY_PROFILE_FUNCTION;
			Context->drawIndexedPrimitives( 
				GeometryBinding,
				ProgramBinding,
				RenderState,
				FrameBuffer,
				&Viewport,
				nullptr,
				RsTopologyType::TRIANGLE_STRIP, 0, 4, 0 );
		} );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaFullscreenQuadComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ),
		getFullName().c_str() );

	BcU32 IndexBufferSize = sizeof( BcU16 ) * 4;
	IndexBuffer_ = RsCore::pImpl()->createBuffer(
		RsBufferDesc( RsResourceBindFlags::INDEX_BUFFER, RsResourceCreationFlags::STATIC, IndexBufferSize ),
		getFullName().c_str() );

	RsCore::pImpl()->updateBuffer( 
		IndexBuffer_.get(), 0, IndexBufferSize, RsResourceUpdateFlags::ASYNC,
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
			RsResourceBindFlags::VERTEX_BUFFER,
			RsResourceCreationFlags::STATIC,
			VertexBufferSize ),
		getFullName().c_str() );

	VertexDeclaration_ = RsCore::pImpl()->createVertexDeclaration( RsVertexDeclarationDesc( 5 )
		.addElement( RsVertexElement( 0,  0, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::POSITION, 0 ) )
		.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::NORMAL, 0 ) )
		.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::TANGENT, 0 ) )
		.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32,    RsVertexUsage::COLOUR, 0 ) )
		.addElement( RsVertexElement( 0, 64, 2, RsVertexDataType::FLOAT32,    RsVertexUsage::TEXCOORD, 0 ) ),
		getFullName().c_str() );

	const auto& Features = RsCore::pImpl()->getContext( 0 )->getFeatures();
	const auto RTOrigin = Features.RTOrigin_;
	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_.get(),
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[ RTOrigin ]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			
			// TODO: Pass in separate UVs for what is intended to be a render target source?
			if( RTOrigin == RsFeatureRenderTargetOrigin::BOTTOM_LEFT )
			{
				*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
				*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
				*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
				*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
			}
			else
			{
				*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
				*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
				*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
				*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( 0.0f, 0.0f, 1.0f,  1.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
			}
		} );

	RsGeometryBindingDesc GeometryBindingDesc;
	GeometryBindingDesc.setVertexDeclaration( VertexDeclaration_.get() );
	GeometryBindingDesc.setVertexBuffer( 0, VertexBuffer_.get(), sizeof( GaVertex ) );
	GeometryBindingDesc.setIndexBuffer( IndexBuffer_.get() );
	GeometryBinding_ = RsCore::pImpl()->createGeometryBinding( GeometryBindingDesc, getFullName().c_str() );

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
void GaFullscreenQuadComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	Parent->detach( MaterialComponent_ );
	MaterialComponent_ = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaFullscreenQuadComponent::getAABB() const
{
	return MaAABB();
}
