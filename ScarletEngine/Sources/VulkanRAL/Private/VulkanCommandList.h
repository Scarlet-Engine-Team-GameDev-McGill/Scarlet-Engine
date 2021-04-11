#pragma once
#include "RALCommandList.h"

#include <vulkan/vulkan.h>

namespace ScarletEngine
{
    class VulkanCommandList : public RALCommandList
    {
    public:
        VulkanCommandList(VkCommandBuffer InCmdBuff)
            : CmdBuff(InCmdBuff)
        {
        }

        VkCommandBuffer CmdBuff;
    };
}
