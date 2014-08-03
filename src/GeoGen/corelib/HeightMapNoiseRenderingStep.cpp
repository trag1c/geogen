#include "HeightMapNoiseRenderingStep.hpp"
#include "../renderer/Renderer.hpp"
#include "../renderer/RendererObject.hpp"
#include "../InternalErrorException.hpp"
#include "../genlib/HeightMap.hpp"
#include "../renderer/RenderingBounds2D.hpp"

using namespace geogen;
using namespace renderer;
using namespace corelib;
using namespace genlib;

void HeightMapNoiseRenderingStep::Step(Renderer* renderer) const
{
	HeightMap* profile = new HeightMap(dynamic_cast<RenderingBounds2D*>(renderer->GetRenderingSequenceMetadata().GetRenderingBounds(this))->GetRectangle(), 0, renderer->GetRenderingSequence().GetRenderScale());
	profile->Noise(this->layers, this->seed);
	
	RendererObject* object = new RendererObject(RENDERER_OBJECT_TYPE_HEIGHT_MAP, profile);

	renderer->GetObjectTable().SetObject(this->GetReturnSlot(), object);
}