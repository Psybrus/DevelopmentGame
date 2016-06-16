/**************************************************************************
*
* File:		GaTestTextureComponent.cpp
* Author:	Neil Richardson 
* Ver/Date:	
* Description:
*		
*		
*
*
* 
**************************************************************************/

#include "GaTestTextureComponent.h"

#include "System/Scene/Rendering/ScnShaderFileData.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsPackage.h"
#include "System/Content/CsCore.h"

#include "System/SysKernel.h"

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
// GaTestTextureBlockData
REFLECTION_DEFINE_BASIC( GaTestTextureBlockData );

void GaTestTextureBlockData::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "UVWOffset_", &GaTestTextureBlockData::UVWOffset_ ),
	};

	auto& Class = ReRegisterClass< GaTestTextureBlockData >( Fields );
	Class.addAttribute( new ScnShaderDataAttribute( "TextureTest", BcFalse ) );
}

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTestTextureComponent );

void GaTestTextureComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material1D_", &GaTestTextureComponent::Material1D_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "Material2D_", &GaTestTextureComponent::Material2D_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "Material3D_", &GaTestTextureComponent::Material3D_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),
		new ReField( "MaterialCube_", &GaTestTextureComponent::MaterialCube_, bcRFF_SHALLOW_COPY | bcRFF_IMPORTER ),

		new ReField( "ObjectUniformBuffer_", &GaTestTextureComponent::ObjectUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "TestUniformBuffer_", &GaTestTextureComponent::TestUniformBuffer_, bcRFF_TRANSIENT ),
		new ReField( "IndexBuffer_", &GaTestTextureComponent::IndexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexBuffer_", &GaTestTextureComponent::VertexBuffer_, bcRFF_TRANSIENT ),
		new ReField( "VertexDeclaration_", &GaTestTextureComponent::VertexDeclaration_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent1D_", &GaTestTextureComponent::MaterialComponent1D_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent2D_", &GaTestTextureComponent::MaterialComponent2D_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent3D_", &GaTestTextureComponent::MaterialComponent3D_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponentCube_", &GaTestTextureComponent::MaterialComponentCube_, bcRFF_TRANSIENT ),
		new ReField( "Texture1D_", &GaTestTextureComponent::Texture1D_, bcRFF_TRANSIENT ),
		new ReField( "Texture2D_", &GaTestTextureComponent::Texture2D_, bcRFF_TRANSIENT ),
		new ReField( "Texture3D_", &GaTestTextureComponent::Texture3D_, bcRFF_TRANSIENT ),
		new ReField( "TextureCube_", &GaTestTextureComponent::TextureCube_, bcRFF_TRANSIENT ),
	};
		
	ReRegisterClass< GaTestTextureComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor() );
}

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTestTextureComponent::GaTestTextureComponent():
	ObjectUniformBuffer_( nullptr ),
	TestUniformBuffer_( nullptr ),
	IndexBuffer_( nullptr ),
	VertexBuffer_( nullptr ),
	VertexDeclaration_( nullptr ),
	Texture1D_( nullptr ),
	Texture2D_( nullptr ),
	Texture3D_( nullptr ),
	TextureCube_( nullptr )
{
}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTestTextureComponent::~GaTestTextureComponent()
{
}

//////////////////////////////////////////////////////////////////////////
// drawTest
void GaTestTextureComponent::drawTest(
		const MaMat4d& Transform, 
		ScnMaterialComponent* Material,
		ScnRenderContext & RenderContext )
{
	if( Material )
	{
		// Set parameters.
		Material->setObjectUniformBlock( ObjectUniformBuffer_.get() );
		Material->setUniformBlock( "GaTestTextureBlockData", TestUniformBuffer_.get() );
		
		// Set material components for view.
		RenderContext.pViewComponent_->setMaterialParameters( Material );
				
		// Render primitive.
		RenderContext.pFrame_->queueRenderNode( RenderContext.Sort_,
			[
				ObjectUniformBuffer = ObjectUniformBuffer_.get(),
				GeometryBinding = GeometryBinding_.get(),
				ProgramBinding = Material->getProgramBinding(),
				RenderState = Material->getRenderState(),
				FrameBuffer = RenderContext.pViewComponent_->getFrameBuffer(),
				Viewport = RenderContext.pViewComponent_->getViewport(),
				Transform 
			]
			( RsContext* Context )
			{
				PSY_PROFILER_SECTION( RenderRoot, "GaTestTextureComponentRenderNode::render" );

				Context->updateBuffer( 
					ObjectUniformBuffer,
					0, sizeof( ScnShaderObjectUniformBlockData ),
					RsResourceUpdateFlags::ASYNC,
					[ Transform ]( RsBuffer* Buffer, const RsBufferLock& Lock )
					{
						auto UniformBlock = reinterpret_cast< ScnShaderObjectUniformBlockData* >( Lock.Buffer_ );
						UniformBlock->WorldTransform_ = Transform;
						UniformBlock->NormalTransform_ = Transform;
					} );

				Context->drawIndexedPrimitives( 
					GeometryBinding,
					ProgramBinding,
					RenderState,
					FrameBuffer,
					&Viewport,
					nullptr,
					RsTopologyType::TRIANGLE_STRIP, 0, 4, 0, 0, 1 );
			} );
	}
}

//////////////////////////////////////////////////////////////////////////
// render
//virtual 
void GaTestTextureComponent::render( ScnRenderContext & RenderContext )
{
	Super::render( RenderContext );

	RsCore::pImpl()->updateBuffer( 
		TestUniformBuffer_.get(),
		0, sizeof( GaTestTextureBlockData ),
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			static BcF32 Timer = 0.0f;
			Timer += SysKernel::pImpl()->getFrameTime() * 0.05f;
			auto UniformBlock = reinterpret_cast< GaTestTextureBlockData* >( Lock.Buffer_ );
			UniformBlock->UVWOffset_ = MaVec4d( Timer, Timer * 0.5f, Timer * 2.0f, 0.0f );
		} );

	MaMat4d Transform;

	Transform.translation( MaVec3d( -2.0f, 0.0f, 0.0f ) );
	drawTest( Transform, MaterialComponent1D_, RenderContext );

	Transform.translation( MaVec3d( 0.0f, 0.0f, 0.0f ) );
	drawTest( Transform, MaterialComponent2D_, RenderContext );

	Transform.translation( MaVec3d( 2.0f, 0.0f, 0.0f ) );
	drawTest( Transform, MaterialComponent3D_, RenderContext );

	static BcF32 Timer = 0.0f;
	Timer += SysKernel::pImpl()->getFrameTime() * 0.25f;
	Transform.rotation( MaVec3d( Timer, Timer * 0.25f, Timer * 0.05f ) );
	Transform.translation( MaVec3d( 4.0f, 0.0f, 0.0f ) );
	drawTest( Transform, MaterialComponentCube_, RenderContext );
}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTestTextureComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );


	ObjectUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( ScnShaderObjectUniformBlockData ) ),
		getFullName().c_str() );


	TestUniformBuffer_ = RsCore::pImpl()->createBuffer( 
		RsBufferDesc(
			RsResourceBindFlags::UNIFORM_BUFFER,
			RsResourceCreationFlags::STREAM,
			sizeof( GaTestTextureBlockData ) ),
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

	RsCore::pImpl()->updateBuffer( 
		VertexBuffer_.get(),
		0, VertexBufferSize,
		RsResourceUpdateFlags::ASYNC,
		[]( RsBuffer* Buffer, const RsBufferLock& Lock )
		{
			auto Vertices = reinterpret_cast< GaVertex* >( Lock.Buffer_ );
			*Vertices++ = GaVertex( MaVec4d( -1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( -1.0f, -1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f, -1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d(  1.0f, -1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 0.0f ) );
			*Vertices++ = GaVertex( MaVec4d( -1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d( -1.0f,  1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 0.0f, 1.0f ) );
			*Vertices++ = GaVertex( MaVec4d(  1.0f,  1.0f,  0.0f,  1.0f ) * 10.0f, MaVec4d(  1.0f,  1.0f, 1.0f,  0.0f ), MaVec4d( 1.0f, 0.0f, 0.0f,  1.0f ), MaVec4d( 1.0f, 1.0f, 1.0f, 1.0f ), MaVec2d( 1.0f, 1.0f ) );
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

	const auto& Features = RsCore::pImpl()->getContext( nullptr )->getFeatures();

	// Setup materials & textures.
	if( Material1D_ )
	{
		MaterialComponent1D_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, Material1D_, ShaderPermutation );

		// Create texture.
		if( Features.Texture1D_ )
		{
			Texture1D_ = ScnTexture::New1D( 32, 1, RsTextureFormat::R8G8B8A8, getFullName().c_str() );
			auto Slice = Texture1D_->getTexture()->getSlice( 0 );
			RsCore::pImpl()->updateTexture( 
				Texture1D_->getTexture(),
				Slice,
				RsResourceUpdateFlags::ASYNC,
				[]( RsTexture* Texture, const RsTextureLock& Lock )
				{
					const auto& Desc = Texture->getDesc();
					BcU32* Data = reinterpret_cast< BcU32* >( 
						reinterpret_cast< BcU8* >( Lock.Buffer_ ) );
					for( BcU32 X = 0; X < Desc.Width_; ++X )
					{
						const BcU32 XDiv = X / 4;
						*Data++ = ( ( ( XDiv ) & 1 ) == 0 ) ? 0xffffffff : 0xff000000;
					}
				} );

			// Bind.
			MaterialComponent1D_->setTexture( "aDiffuseTex", Texture1D_ );
		}
	}
	if( Material2D_ )
	{
		MaterialComponent2D_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, Material2D_, ShaderPermutation );

		// Create texture.
		if( Features.Texture2D_ )
		{
			Texture2D_ = ScnTexture::New2D( 32, 32, 1, RsTextureFormat::R8G8B8A8, false, false, getFullName().c_str() );
			auto Slice = Texture2D_->getTexture()->getSlice( 0 );
			RsCore::pImpl()->updateTexture( 
				Texture2D_->getTexture(),
				Slice,
				RsResourceUpdateFlags::ASYNC,
				[]( RsTexture* Texture, const RsTextureLock& Lock )
				{
					const auto& Desc = Texture->getDesc();
					for( BcU32 Y = 0; Y < Desc.Height_; ++Y )
					{
						BcU32* Data = reinterpret_cast< BcU32* >( 
							reinterpret_cast< BcU8* >( Lock.Buffer_ ) + Y * Lock.Pitch_ );
						for( BcU32 X = 0; X < Desc.Width_; ++X )
						{
							const BcU32 XDiv = X / 4;
							const BcU32 YDiv = Y / 4;
							*Data++ = ( ( ( XDiv + YDiv ) & 1 ) == 0 ) ? 0xffffffff : 0xff000000;
						}
					}
				} );

			// Bind.
			MaterialComponent2D_->setTexture( "aDiffuseTex", Texture2D_ );
		}
	}
	if( Material3D_ )
	{
		MaterialComponent3D_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, Material3D_, ShaderPermutation );

		// Create texture.
		if( Features.Texture3D_ )
		{
			Texture3D_ = ScnTexture::New3D( 32, 32, 32, 1, RsTextureFormat::R8G8B8A8, getFullName().c_str() );
			auto Slice = Texture3D_->getTexture()->getSlice( 0 );
			RsCore::pImpl()->updateTexture( 
				Texture3D_->getTexture(),
				Slice,
				RsResourceUpdateFlags::ASYNC,
				[]( RsTexture* Texture, const RsTextureLock& Lock )
				{
					const auto& Desc = Texture->getDesc();
					for( BcU32 Z = 0; Z < Desc.Depth_; ++Z )
					{
						BcU32* SliceData = reinterpret_cast< BcU32* >( 
							reinterpret_cast< BcU8* >( Lock.Buffer_ ) + Z * Lock.SlicePitch_ );
						for( BcU32 Y = 0; Y < Desc.Height_; ++Y )
						{
							BcU32* Data = reinterpret_cast< BcU32* >( 
								reinterpret_cast< BcU8* >( SliceData ) + Y * Lock.Pitch_ );
							for( BcU32 X = 0; X < Desc.Width_; ++X )
							{
								const BcU32 XDiv = X / 4;
								const BcU32 YDiv = Y / 4;
								const BcU32 ZDiv = Z / 4;
								*Data++ = ( ( ( XDiv + YDiv + ZDiv ) & 1 ) == 0 ) ? 0xffffffff : 0xff000000;
							}
						}
					}
				} );

			// Bind.
			MaterialComponent3D_->setTexture( "aDiffuseTex", Texture3D_ );
		}
	}
	if( MaterialCube_ )
	{
		MaterialComponentCube_ = Parent->attach< ScnMaterialComponent >(
			BcName::INVALID, MaterialCube_, ShaderPermutation );

		// Create texture.
		if( Features.TextureCube_ )
		{
			TextureCube_ = ScnTexture::NewCube( 32, 32, 1, RsTextureFormat::R8G8B8A8, getFullName().c_str() );
			const BcU32 FaceColours[] = 
			{
				0xffff0000,
				0xff00ffff,
				0xff00ff00,
				0xffff00ff,
				0xff0000ff,
				0xffffff00,
			};

			const RsTextureFace Faces[] = 
			{
				RsTextureFace::POSITIVE_X,
				RsTextureFace::NEGATIVE_X,
				RsTextureFace::POSITIVE_Y,
				RsTextureFace::NEGATIVE_Y,
				RsTextureFace::POSITIVE_Z,
				RsTextureFace::NEGATIVE_Z,
			};

			for( BcU32 Face = 0; Face < 6; ++Face )
			{
				const BcU32 Colour = FaceColours[ Face ];
				auto Slice = TextureCube_->getTexture()->getSlice( 0, Faces[ Face ] );
				RsCore::pImpl()->updateTexture( 
					TextureCube_->getTexture(),
					Slice,
					RsResourceUpdateFlags::ASYNC,
					[ Colour ]( RsTexture* Texture, const RsTextureLock& Lock )
					{
						const auto& Desc = Texture->getDesc();
						for( BcU32 Y = 0; Y < Desc.Height_; ++Y )
						{
							BcU32* Data = reinterpret_cast< BcU32* >( 
								reinterpret_cast< BcU8* >( Lock.Buffer_ ) + Y * Lock.Pitch_ );
							for( BcU32 X = 0; X < Desc.Width_; ++X )
							{
								const BcU32 XDiv = X / 4;
								const BcU32 YDiv = Y / 4;
								*Data++ = ( ( ( XDiv + YDiv ) & 1 ) == 0 ) ? Colour : 0xff000000;
							}
						}
					} );
			}
			// Bind.
			MaterialComponentCube_->setTexture( "aDiffuseTex", TextureCube_ );
		}
	}

}
	
//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTestTextureComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	if(	Texture1D_ )
	{
		Texture1D_->markDestroy();
		Texture1D_ = nullptr;
	}

	if(	Texture2D_ )
	{
		Texture2D_->markDestroy();
		Texture2D_ = nullptr;
	}

	if(	Texture3D_ )
	{
		Texture3D_->markDestroy();
		Texture3D_ = nullptr;
	}

	if(	TextureCube_ )
	{
		TextureCube_->markDestroy();
		TextureCube_ = nullptr;
	}

	if( MaterialComponent1D_ )
	{
		Parent->detach( MaterialComponent1D_ );
		MaterialComponent1D_ = nullptr;
	}

	if( MaterialComponent2D_ )
	{
		Parent->detach( MaterialComponent2D_ );
		MaterialComponent2D_ = nullptr;
	}

	if( MaterialComponent3D_ )
	{
		Parent->detach( MaterialComponent3D_ );
		MaterialComponent3D_ = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTestTextureComponent::getAABB() const
{
	return MaAABB();
}
