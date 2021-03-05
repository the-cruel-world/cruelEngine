#include "../object.hpp"
#include "../../render/render_task.hpp"
#include "../../render/vkcommon.h"

namespace cruelEngine {
namespace cruelScene
{
    class TriangleTask : public cruelRender::RenderTask {
    public:
        TriangleTask(cruelRender::RenderSession& session, cruelRender::RenderPass &render_pass, const VkBuffer &vertexBuffer) : 
            cruelRender::RenderTask(session, render_pass), vertexBuffer{vertexBuffer}
        {

        }
        void        draw(cruelRender::CommandBuffer &commandBuffer);
    private:
        const VkBuffer        &vertexBuffer;
    };

    class Triangle : public Object {
    public:
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 color;
            
            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);
                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, color);
                return attributeDescriptions;
            }
        };

        Triangle(cruelRender::RenderContext &render_context, u32 session_idx, std::string name);
        ~Triangle();

        void prepare();

        void createVertexBuffer();

    private:

        const std::vector<Vertex> vertices = {
            {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}
        };
        VkBuffer                    vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory              vertexBufferMemory = VK_NULL_HANDLE;
    };
}
}