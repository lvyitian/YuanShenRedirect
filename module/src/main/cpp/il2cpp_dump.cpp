//
// Created by Perfare on 2020/7/4.
//

#include "il2cpp_dump.h"
#include "open_hook.h"
#include <bytehook.h>
#include <dlfcn.h>
#include <cstdlib>
#include <cstring>
#include <cinttypes>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include "xdl.h"
#include "log.h"

#define DO_API(r, n, p) r (*n) p

#undef DO_API

static uint64_t il2cpp_base = 0;

void init_il2cpp_api(void *handle) {
    bytehook_init(0,false);
#define DO_API(r, n, p) {                      \
    n = (r (*) p)xdl_sym(handle, #n, nullptr); \
    if(!n) {                                   \
        LOGW("api not found %s", #n);          \
    }                                          \
}
#undef DO_API
}

void il2cpp_api_init(void *handle) {
    LOGI("il2cpp_handle: %p", handle);
    init_il2cpp_api(handle);
}

void il2cpp_dump(const char *outDir) {
    LOGI("hooking...");
    bytehook_hook_all(NULL, "open", (void*)&open_hook, NULL, NULL);
    bytehook_hook_all(NULL, "__open_real", (void*)&open_real_hook, NULL, NULL);
    bytehook_hook_all(NULL, "__open_2", (void*)&open2_hook, NULL, NULL);
    bytehook_hook_all(NULL, "opendir", (void*)&opendir_hook, NULL, NULL);
    bytehook_hook_all(NULL,"access",(void*)&access_hook,NULL,NULL);
    bytehook_hook_all(NULL,"unlink",(void*)&unlink_hook,NULL,NULL);
    bytehook_hook_all(NULL,"rmdir",(void*)&rmdir_hook,NULL,NULL);
    bytehook_hook_all(NULL,"remove",(void*)&remove_hook,NULL,NULL);
    bytehook_hook_all(NULL,"create",(void*)&create_hook,NULL,NULL);
    bytehook_hook_all(NULL,"mkdir",(void*)&mkdir_hook,NULL,NULL);
    bytehook_hook_all(NULL,"link",(void*)&link_hook,NULL,NULL);
    bytehook_hook_all(NULL,"symlink",(void*)&symlink_hook,NULL,NULL);
    bytehook_hook_all(NULL,"readlink",(void*)&readlink_hook,NULL,NULL);
    bytehook_hook_all(NULL,"stat",(void*)&stat_hook,NULL,NULL);
    bytehook_hook_all(NULL,"lstat",(void*)&lstat_hook,NULL,NULL);
    bytehook_hook_all(NULL,"ftw",(void*)&ftw_hook,NULL,NULL);
    bytehook_hook_all(NULL,"statvfs",(void*)&statvfs_hook,NULL,NULL);

    /*size_t size;
    auto domain = il2cpp_domain_get();
    auto assemblies = il2cpp_domain_get_assemblies(domain, &size);
    std::stringstream imageOutput;
    for (int i = 0; i < size; ++i) {
        auto image = il2cpp_assembly_get_image(assemblies[i]);
        imageOutput << "// Image " << i << ": " << il2cpp_image_get_name(image) << "\n";
    }
    std::vector<std::string> outPuts;
    if (il2cpp_image_get_class) {
        LOGI("Version greater than 2018.3");
        //使用il2cpp_image_get_class
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            imageStr << "\n// Dll : " << il2cpp_image_get_name(image);
            auto classCount = il2cpp_image_get_class_count(image);
            for (int j = 0; j < classCount; ++j) {
                auto klass = il2cpp_image_get_class(image, j);
                auto type = il2cpp_class_get_type(const_cast<Il2CppClass *>(klass));
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    } else {
        LOGI("Version less than 2018.3");
        //使用反射
        auto corlib = il2cpp_get_corlib();
        auto assemblyClass = il2cpp_class_from_name(corlib, "System.Reflection", "Assembly");
        auto assemblyLoad = il2cpp_class_get_method_from_name(assemblyClass, "Load", 1);
        auto assemblyGetTypes = il2cpp_class_get_method_from_name(assemblyClass, "GetTypes", 0);
        if (assemblyLoad && assemblyLoad->methodPointer) {
            LOGI("Assembly::Load: %p", assemblyLoad->methodPointer);
        } else {
            LOGI("miss Assembly::Load");
            return;
        }
        if (assemblyGetTypes && assemblyGetTypes->methodPointer) {
            LOGI("Assembly::GetTypes: %p", assemblyGetTypes->methodPointer);
        } else {
            LOGI("miss Assembly::GetTypes");
            return;
        }
        typedef void *(*Assembly_Load_ftn)(void *, Il2CppString *, void *);
        typedef Il2CppArray *(*Assembly_GetTypes_ftn)(void *, void *);
        for (int i = 0; i < size; ++i) {
            auto image = il2cpp_assembly_get_image(assemblies[i]);
            std::stringstream imageStr;
            auto image_name = il2cpp_image_get_name(image);
            imageStr << "\n// Dll : " << image_name;
            //LOGD("image name : %s", image->name);
            auto imageName = std::string(image_name);
            auto pos = imageName.rfind('.');
            auto imageNameNoExt = imageName.substr(0, pos);
            auto assemblyFileName = il2cpp_string_new(imageNameNoExt.data());
            auto reflectionAssembly = ((Assembly_Load_ftn) assemblyLoad->methodPointer)(nullptr,
                                                                                        assemblyFileName,
                                                                                        nullptr);
            auto reflectionTypes = ((Assembly_GetTypes_ftn) assemblyGetTypes->methodPointer)(
                    reflectionAssembly, nullptr);
            auto items = reflectionTypes->vector;
            for (int j = 0; j < reflectionTypes->max_length; ++j) {
                auto klass = il2cpp_class_from_system_type((Il2CppReflectionType *) items[j]);
                auto type = il2cpp_class_get_type(klass);
                //LOGD("type name : %s", il2cpp_type_get_name(type));
                auto outPut = imageStr.str() + dump_type(type);
                outPuts.push_back(outPut);
            }
        }
    }
    LOGI("write dump file");
    auto outPath = std::string(outDir).append("/files/dump.cs");
    std::ofstream outStream(outPath);
    outStream << imageOutput.str();
    auto count = outPuts.size();
    for (int i = 0; i < count; ++i) {
        outStream << outPuts[i];
    }
    outStream.close();*/
    LOGI("hook done!");
}