#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#pragma warning(disable : 26812)

class HelloTriangleApplication 
{
public:
    void run() 
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);

        createInstance();
    }

    void createInstance()
    {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        createInfo.enabledLayerCount = 0;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create instance!");
        }

        //check optional extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); //get count only
        
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "Available Extensions:\n";
        for (const auto& ext : extensions)
        {
            std::cout << '\t' << ext.extensionName << '\n';
        }

        std::cout << "\nRequired Extensions:\n";
        for (uint32_t i = 0; i < glfwExtensionCount; ++i)
        {
            std::cout << '\t' << glfwExtensions[i] << '\n';
        }

        verifyExtensions(extensions, glfwExtensions, glfwExtensionCount);
    }

    bool verifyExtensions(const std::vector<VkExtensionProperties>& extensions, const char** requiredList, uint32_t requiredCount)
    {
        std::vector<std::string> missingList;

        for (uint32_t i = 0; i < requiredCount; ++i)
        {
            std::string requiredExtension = requiredList[i];
            bool found = false;

            for (const auto& ext : extensions)
            {
                if (requiredExtension == ext.extensionName)
                {
                    found = true;
                    break;
                }
            }

            if (!found)
                missingList.push_back(std::move(requiredExtension));
        }

        if (!missingList.empty())
        {
            std::string msg = "The following extensions are missing:\n";
            for (const auto& extName : missingList)
            {
                msg += '\t' + extName + '\n';
            }
            throw std::runtime_error(msg);
        }
    }

    void initVulkan() 
    {

    }

    void mainLoop() 
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup() 
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    GLFWwindow* window = nullptr;
    VkInstance instance;
};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}