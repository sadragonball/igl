//
// Created by ROG on 2023/11/10.
//

#include "Volume.h"

#include <igl/IGLSafeC.h>
#include <igl/vulkan/CommandBuffer.h>
#include <igl/vulkan/Common.h>
#include <igl/vulkan/Device.h>
#include <igl/vulkan/Texture.h>
#include <igl/vulkan/VulkanContext.h>
#include <igl/vulkan/VulkanImage.h>
#include <igl/vulkan/VulkanImageView.h>
#include <igl/vulkan/VulkanStagingDevice.h>
#include <igl/vulkan/VulkanTexture.h>

namespace igl {
namespace vulkan {
Volume::Volume(const Device& device, TextureFormat format) : ITexture(format), device_(device) {}

Result Volume::create(const TextureDesc& desc) {
  desc_ = desc;

  const VulkanContext& ctx = device_.getVulkanContext();

  const VkFormat vkFormat = getProperties().isDepthOrDepthStencil()
                                ? ctx.getClosestDepthStencilFormat(desc_.format)
                                : textureFormatToVkFormat(desc_.format);

  const igl::TextureType type = desc_.type;
}

Dimensions Volume::getDimensions() const {
  return Dimensions();
}
size_t Volume::getNumLayers() const {
  return 0;
}
TextureType Volume::getType() const {
  return TextureType::TwoD;
}
TextureDesc::TextureUsage Volume::getUsage() const {
  return 0;
}
uint32_t Volume::getSamples() const {
  return 0;
}
uint32_t Volume::getNumMipLevels() const {
  return 0;
}
void Volume::generateMipmap(ICommandQueue& cmdQueue) const {}
void Volume::generateMipmap(ICommandBuffer& cmdBuffer) const {}
bool Volume::isRequiredGenerateMipmap() const {
  return false;
}
uint64_t Volume::getTextureId() const {
  return 0;
}
VkFormat Volume::getVkFormat() const {
  return VK_FORMAT_R32_UINT;
}
VkImageView Volume::getVkImageView() const {
  return nullptr;
}
VkImageView Volume::getVkImageViewForFramebuffer(uint32_t mipLevel,
                                                 uint32_t layer,
                                                 FramebufferMode mode) const {
  return nullptr;
}
VkImage Volume::getVkImage() const {
  return nullptr;
}
bool Volume::isSwapchainTexture() const {
  return false;
}
uint32_t Volume::getNumVkLayers() const {
  return 0;
}

Result Volume::uploadInternal(TextureType type,
                              const TextureRangeDesc& range,
                              const void* data,
                              size_t bytesPerRow) const {
  return ITexture::uploadInternal(type, range, data, bytesPerRow);
}
} // namespace vulkan
} // namespace igl