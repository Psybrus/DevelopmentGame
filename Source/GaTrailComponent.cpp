#include "GaTrailComponent.h"

#include "System/Scene/Rendering/ScnMaterial.h"
#include "System/Scene/Rendering/ScnViewComponent.h"

#include "System/Content/CsCore.h"

#include "System/SysKernel.h"

#include "Base/BcProfiler.h"

//////////////////////////////////////////////////////////////////////////
// Define resource internals.
REFLECTION_DEFINE_DERIVED( GaTrailComponent );

void GaTrailComponent::StaticRegisterClass()
{
	ReField* Fields[] = 
	{
		new ReField( "Material_", &GaTrailComponent::Material_, bcRFF_IMPORTER | bcRFF_SHALLOW_COPY ),
		new ReField( "Width_", &GaTrailComponent::Width_, bcRFF_IMPORTER ),
		new ReField( "MaxLength_", &GaTrailComponent::MaxLength_, bcRFF_IMPORTER ),
		new ReField( "SegmentDistance_", &GaTrailComponent::SegmentDistance_, bcRFF_IMPORTER ),

		new ReField( "DistanceMoved_", &GaTrailComponent::DistanceMoved_, bcRFF_TRANSIENT ),
		new ReField( "MaxTrailHistory_", &GaTrailComponent::MaxTrailHistory_, bcRFF_TRANSIENT ),
		new ReField( "TrailHistory_", &GaTrailComponent::TrailHistory_, bcRFF_TRANSIENT ),
		new ReField( "MaterialComponent_", &GaTrailComponent::MaterialComponent_, bcRFF_TRANSIENT ),
	};

	using namespace std::placeholders;
	ReRegisterClass< GaTrailComponent, Super >( Fields )
		.addAttribute( new ScnComponentProcessor(		{
			ScnComponentProcessFuncEntry(
				"Update trails",
				ScnComponentPriority::MODEL_UPDATE,
				std::bind( &GaTrailComponent::updateTrails, _1 ) ),
		} ) );
}

//////////////////////////////////////////////////////////////////////////
// Statics
SysFence GaTrailComponent::UpdateFence_( 0 );

//////////////////////////////////////////////////////////////////////////
// Ctor
GaTrailComponent::GaTrailComponent():
	Material_( nullptr ),
	Width_( 1.0f ),
	MaxLength_( 100.0f ),
	SegmentDistance_( 1.0f ),
	DistanceMoved_( 0.0f ),
	MaxTrailHistory_( 0 ),
	TrailHistory_(),
	MaterialComponent_( nullptr )
{

}

//////////////////////////////////////////////////////////////////////////
// Dtor
//virtual
GaTrailComponent::~GaTrailComponent()
{

}

//////////////////////////////////////////////////////////////////////////
// onAttach
//virtual
void GaTrailComponent::onAttach( ScnEntityWeakRef Parent )
{
	Super::onAttach( Parent );
	
	// Reserve trail history.
	MaxTrailHistory_ = static_cast< size_t >( std::ceilf( MaxLength_ / SegmentDistance_ ) ) + 1;
	TrailHistory_.reserve( MaxTrailHistory_ );

	size_t NoofVertices = MaxTrailHistory_ * 2;

	VertexDeclaration_.reset( RsCore::pImpl()->createVertexDeclaration(
		RsVertexDeclarationDesc( 4 )
			.addElement( RsVertexElement( 0, 0, 4, RsVertexDataType::FLOAT32, RsVertexUsage::POSITION, 0 ) )
			.addElement( RsVertexElement( 0, 16, 4, RsVertexDataType::FLOAT32, RsVertexUsage::NORMAL, 0 ) )
			.addElement( RsVertexElement( 0, 32, 4, RsVertexDataType::FLOAT32, RsVertexUsage::TEXCOORD, 0 ) )
			.addElement( RsVertexElement( 0, 48, 4, RsVertexDataType::FLOAT32, RsVertexUsage::COLOUR, 0 ) ) ) );

	VertexBuffer_.reset( RsCore::pImpl()->createBuffer(
		RsBufferDesc( 
			RsBufferType::VERTEX,
			RsResourceCreationFlags::DYNAMIC,
			NoofVertices * sizeof( GaTrailVertex ) ) ) );

	UniformBuffer_.reset( RsCore::pImpl()->createBuffer( 
		RsBufferDesc( 
			RsBufferType::UNIFORM,
			RsResourceCreationFlags::STREAM,
			sizeof( ObjectUniforms_ ) ) ) );

	MaterialComponent_ = Parent->attach< ScnMaterialComponent >( 
		BcName::INVALID, Material_, 
		ScnShaderPermutationFlags::MESH_STATIC_3D );
}

//////////////////////////////////////////////////////////////////////////
// onDetach
//virtual
void GaTrailComponent::onDetach( ScnEntityWeakRef Parent )
{
	Super::onDetach( Parent );

	UpdateFence_.wait();

	Parent->detach( MaterialComponent_ );

	VertexDeclaration_.reset();
	VertexBuffer_.reset();
	UniformBuffer_.reset();
}

//////////////////////////////////////////////////////////////////////////
// getAABB
//virtual
MaAABB GaTrailComponent::getAABB() const
{
	return MaAABB();
}

//////////////////////////////////////////////////////////////////////////
// render
void GaTrailComponent::render( ScnRenderContext & RenderContext )
{
	if( TrailHistory_.size() > 1 )
	{
		UpdateFence_.wait();

		RsRenderSort Sort = RenderContext.Sort_;
		Sort.Layer_ = 15;

		RenderContext.pViewComponent_->setMaterialParameters( MaterialComponent_ );
		MaterialComponent_->bind( RenderContext.pFrame_, Sort );
		RenderContext.pFrame_->queueRenderNode( Sort,
			[ this ]( RsContext* Context )
			{
				const BcU32 NoofIndices = static_cast< BcU32 >( TrailHistory_.size() * 2 );
				Context->setVertexBuffer( 0, VertexBuffer_.get(), sizeof( GaTrailVertex ) );
				Context->setVertexDeclaration( VertexDeclaration_.get() );
				Context->drawPrimitives( RsTopologyType::TRIANGLE_STRIP, 0, NoofIndices );
			} );
	}
}

//////////////////////////////////////////////////////////////////////////
// updateTrails
//static
void GaTrailComponent::updateTrails( const ScnComponentList& Components )
{
	PSY_PROFILE_FUNCTION;

	for( auto InComponent : Components )
	{
		BcAssert( InComponent->isTypeOf< GaTrailComponent >() );
		auto* Component = static_cast< GaTrailComponent* >( InComponent.get() );

		// Set current, and grab last for comparison.
		BcF32 DistanceMoved = 0.0f;
		const auto& ThisWorldMatrix = Component->getParentEntity()->getWorldMatrix();
		if( Component->TrailHistory_.size() > 0 )
		{
			auto& CurrWorldMatrix = Component->TrailHistory_[ Component->TrailHistory_.size() - 1 ];
			DistanceMoved = ( CurrWorldMatrix.translation() - ThisWorldMatrix.translation() ).magnitude();
			CurrWorldMatrix = ThisWorldMatrix;
		}

		// Calculate distance moved from previous segment created.
		Component->DistanceMoved_ += DistanceMoved;

		// Add a new segment.
		// TODO: Move first element towards second when approaching end of trail.
		if( Component->DistanceMoved_ > Component->SegmentDistance_ ||
			Component->TrailHistory_.size() == 0 )
		{
			if( Component->TrailHistory_.size() == Component->MaxTrailHistory_ )
			{
				Component->TrailHistory_.erase( Component->TrailHistory_.begin() );
			}

			Component->TrailHistory_.push_back( ThisWorldMatrix );
			Component->DistanceMoved_ -= Component->SegmentDistance_;
		}
		
		if( Component->TrailHistory_.size() > 1 )
		{
			// Update fence.
			UpdateFence_.increment();

			// Bake uniform buffer.
			RsCore::pImpl()->updateBuffer( 
				Component->UniformBuffer_.get(),
				0, sizeof( Component->ObjectUniforms_ ),
				RsResourceUpdateFlags::ASYNC,
				[ Component ]( RsBuffer* Buffer, const RsBufferLock& Lock )
				{
					Component->ObjectUniforms_.WorldTransform_.identity();
					Component->ObjectUniforms_.NormalTransform_.identity();

					BcMemCopy( Lock.Buffer_, &Component->ObjectUniforms_, sizeof( Component->ObjectUniforms_ ) );
				} );

			// Bake new vertex buffer.
			// TODO: Partial update perhaps?
			RsCore::pImpl()->updateBuffer(
				Component->VertexBuffer_.get(), 
				0, Component->VertexBuffer_->getDesc().SizeBytes_, 
				RsResourceUpdateFlags::ASYNC, 
				[ Component ]( RsBuffer* Buffer, RsBufferLock Lock )
				{
					auto Vertices = static_cast< GaTrailVertex* >( Lock.Buffer_ );

					BcF32 TexCoordU( 0.0f );

					// Add first 2 vertices.
					{
						const auto& PrevMatrix = Component->TrailHistory_[ 0 ];
						const auto& CurrMatrix = Component->TrailHistory_[ 1 ];

						MaVec3d Difference = ( CurrMatrix.translation() - PrevMatrix.translation() );
						BcF32 Distance = Difference.magnitude();
						MaVec3d Normal = Difference.normal();
						Vertices->Position_ = MaVec4d( PrevMatrix.translation(), 1.0f );
						Vertices->Normal_ = MaVec4d( Normal, 0.0f );
						Vertices->TexCoord_ = MaVec4d( 0.0f, TexCoordU, -1.0f, Component->Width_ );
						Vertices->Colour_ = RsColour::WHITE;
						++Vertices;

						Vertices->Position_ = MaVec4d( PrevMatrix.translation(), 1.0f );
						Vertices->Normal_ = MaVec4d( Normal, 0.0f );
						Vertices->TexCoord_ = MaVec4d( 1.0f, TexCoordU, 1.0f, Component->Width_ );
						Vertices->Colour_ = RsColour::WHITE;
						++Vertices;

						TexCoordU += Distance;
					}

					// Add remaining vertices.
					for( size_t Idx = 1; Idx < Component->TrailHistory_.size(); ++Idx )
					{
						const auto& PrevMatrix = Component->TrailHistory_[ Idx - 1 ];
						const auto& CurrMatrix = Component->TrailHistory_[ Idx ];

						MaVec3d Difference = ( CurrMatrix.translation() - PrevMatrix.translation() );
						BcF32 Distance = Difference.magnitude();
						MaVec3d Normal = Difference.normal();
						Vertices->Position_ = MaVec4d( CurrMatrix.translation(), 1.0f );
						Vertices->Normal_ = MaVec4d( Normal, 0.0f );
						Vertices->TexCoord_ = MaVec4d( 0.0f, TexCoordU, -1.0f, Component->Width_ );
						Vertices->Colour_ = RsColour::WHITE;
						++Vertices;

						Vertices->Position_ = MaVec4d( CurrMatrix.translation(), 1.0f );
						Vertices->Normal_ = MaVec4d( Normal, 0.0f );
						Vertices->TexCoord_ = MaVec4d( 1.0f, TexCoordU, 1.0f, Component->Width_ );
						Vertices->Colour_ = RsColour::WHITE;
						++Vertices;

						TexCoordU += Distance;
					}

					BcAssert( (BcU8*)Vertices <= (BcU8*)Lock.Buffer_ + Buffer->getDesc().SizeBytes_ );

					UpdateFence_.decrement();
				} );
		}
	}
}
