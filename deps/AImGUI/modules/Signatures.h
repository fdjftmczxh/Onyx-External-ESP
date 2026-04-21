#pragma once

#include <cstddef>

namespace android::anative_window_creator::signatures
{
    /**
     * @brief Represents a byte pattern signature for multiple ABIs.
     */
    struct MethodSignature
    {
        const char *arm64 = nullptr;
        const char *x86_64 = nullptr;
        const char *arm32 = nullptr;

        [[nodiscard]] const char *Get() const
        {
#if defined(__aarch64__)
            return arm64;
#elif defined(__x86_64__)
            return x86_64;
#elif defined(__arm__)
            return arm32;
#else
            return nullptr;
#endif
        }
    };

    /**
     * @brief Registry of all signatures required for AImGUI.
     */
    struct Registry
    {
        // libutils.so
        MethodSignature RefBase_incStrong                  = { .arm64 = "signature-here", .x86_64 = "48 8B 47 ? F0 83 40 ? ? B9 ? ? ? ? ? ? ? ? 81 F9", .arm32 = "signature-here" }; // _ZNK7android7RefBase9incStrongEPKv
        MethodSignature RefBase_decStrong                  = { .arm64 = "signature-here", .x86_64 = "41 57 41 56 53 48 8B 5F", .arm32 = "signature-here" }; // _ZNK7android7RefBase9decStrongEPKv
        MethodSignature String8_Constructor                = { .arm64 = "signature-here", .x86_64 = "41 57 41 56 41 55 41 54 53 49 89 F7 49 89 FE 48 89 F7 E8 ? ? ? ? 48 83 F8", .arm32 = "signature-here" }; // _ZN7android7String8C2EPKc
        MethodSignature String8_Destructor                 = { .arm64 = "signature-here", .x86_64 = "48 8B 07 48 8D 78 E8 48 85 C0 48 0F 44 F8 31 F6 E9", .arm32 = "signature-here" }; // _ZN7android7String8D2Ev

        // libgui.so
        MethodSignature LayerMetadata_Constructor          = { .arm64 = "signature-here", .x86_64 = "48 8B 05 ? ? ? ? 48 83 C0 ? ? ? ? 0F 57 C0 0F 11 47 ? 0F 11 47 ? C7 47 ? ? ? ? ? C3", .arm32 = "signature-here" }; // _ZN7android13LayerMetadataC2Ev OR _ZN7android3gui13LayerMetadataC2Ev
        MethodSignature LayerMetadata_SetInt32             = { .arm64 = "signature-here", .x86_64 = "55 41 56 53 48 81 EC ? ? ? ? 41 89 D6 64 48 8B 04 25 ? ? ? ? 48 89 84 24", .arm32 = "signature-here" }; // _ZN7android13LayerMetadata8setInt32Eji OR _ZN7android3gui13LayerMetadata8setInt32Eji
        MethodSignature SurfaceComposerClient_Constructor  = { .arm64 = "signature-here", .x86_64 = "53 48 89 FB E8 ? ? ? ? 48 8B 05 ? ? ? ? 48 83 C0 ? ? ? ? 48 8D 7B", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClientC2Ev
        
        // createSurface variants (Version specific)
        MethodSignature CreateSurface_v7                   = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8Ejjij
        MethodSignature CreateSurface_v8                   = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlEjj
        MethodSignature CreateSurface_v9                   = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlEii
        MethodSignature CreateSurface_v10                  = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlENS_13LayerMetadataE
        MethodSignature CreateSurface_v11                  = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlENS_13LayerMetadataEPj
        MethodSignature CreateSurface_v12                  = { .arm64 = "signature-here", .x86_64 = "55 41 57 41 56 41 55 41 54 53 48 83 EC ? 45 89 CE", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijRKNS_2spINS_7IBinderEEENS_13LayerMetadataEPj
        MethodSignature CreateSurface_v13                  = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijRKNS_2spINS_7IBinderEEENS_13LayerMetadataEPj
        MethodSignature CreateSurface_v14                  = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjiiRKNS_2spINS_7IBinderEEENS_3gui13LayerMetadataEPj

        MethodSignature MirrorSurface                      = { .arm64 = "signature-here", .x86_64 = "55 48 89 E5 41 57 41 56 41 54 53 48 83 EC ? 49 89 FE 64 48 8B 04 25 ? ? ? ? 48 89 45 ? 48 85 D2", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient13mirrorSurfaceEPNS_14SurfaceControlE
        MethodSignature GetBuiltInDisplay                  = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient17getBuiltInDisplayEi
        MethodSignature GetInternalDisplayToken            = { .arm64 = "signature-here", .x86_64 = "55 41 57 41 56 41 55 41 54 53 48 83 EC ? 49 89 FE 64 48 8B 04 25", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient23getInternalDisplayTokenEv
        MethodSignature GetPhysicalDisplayIds              = { .arm64 = "signature-here", .x86_64 = "53 48 83 EC ? 48 89 FB 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 89 E7 E8 ? ? ? ? ? ? ? ? ? ? ? 48 89 DF", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient21getPhysicalDisplayIdsEv
        MethodSignature GetPhysicalDisplayToken            = { .arm64 = "signature-here", .x86_64 = "41 56 53 48 83 EC ? 48 89 F3 49 89 FE 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 8D 7C 24", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient23getPhysicalDisplayTokenENS_17PhysicalDisplayIdE
        MethodSignature GetDisplayInfo                     = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient14getDisplayInfoERKNS_2spINS_7IBinderEEEPNS_11DisplayInfoE
        MethodSignature GetDisplayState                    = { .arm64 = "signature-here", .x86_64 = "41 56 53 48 83 EC ? 49 89 F6 48 89 FB 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 8D 7C 24 ? E8 ? ? ? ? 48 8B 7C 24 ? ? ? ? 48 89 DE 4C 89 F2 FF 90 ? ? ? ? 89 C3 48 8B 7C 24 ? 48 85 FF 74 ? ? ? ? 48 03 78 ? 48 8D 74 24 ? E8 ? ? ? ? 64 48 8B 04 25 ? ? ? ? 48 3B 44 24 ? 75 ? 89 D8 48 83 C4 ? 5B 41 5E C3 E8 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 41 56 53 48 83 EC ? 49 89 F6 48 89 FB 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 8D 7C 24 ? E8 ? ? ? ? 48 8B 7C 24 ? ? ? ? 48 89 DE 4C 89 F2 FF 90 ? ? ? ? 89 C3 48 8B 7C 24 ? 48 85 FF 74 ? ? ? ? 48 03 78 ? 48 8D 74 24 ? E8 ? ? ? ? 64 48 8B 04 25 ? ? ? ? 48 3B 44 24 ? 75 ? 89 D8 48 83 C4 ? 5B 41 5E C3 E8 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 41 56 53 48 83 EC ? 49 89 F6 48 89 FB 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 8D 7C 24 ? E8 ? ? ? ? 48 8B 7C 24 ? ? ? ? 48 89 DE 4C 89 F2 FF 90 ? ? ? ? 89 C3 48 8B 7C 24 ? 48 85 FF 74 ? ? ? ? 48 03 78 ? 48 8D 74 24 ? E8 ? ? ? ? 64 48 8B 04 25 ? ? ? ? 48 3B 44 24 ? 75 ? 89 D8 48 83 C4 ? 5B 41 5E C3 E8 ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? 55", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient15getDisplayStateERKNS_2spINS_7IBinderEEEPNS_2ui12DisplayStateE

        // Transaction variants
        MethodSignature Transaction_CopyConstructor        = { .arm64 = "signature-here", .x86_64 = "41 57 41 56 41 54 53 50 49 89 F6 48 89 FB 48 8B 05", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11TransactionC2ERKS1_
        MethodSignature Transaction_Constructor            = { .arm64 = "signature-here", .x86_64 = "53 48 89 FB 48 8B 05 ? ? ? ? 48 83 C0 ? ? ? ? 0F 57 C0", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11TransactionC2Ev
        MethodSignature Transaction_SetLayer               = { .arm64 = "signature-here", .x86_64 = "55 41 56 53 41 89 D6 48 89 F3 48 89 FD E8 ? ? ? ? 48 8B 48", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction8setLayerERKNS_2spINS_14SurfaceControlEEEi
        MethodSignature Transaction_SetTrustedOverlay      = { .arm64 = "signature-here", .x86_64 = "55 53 50 89 D5 48 89 FB E8 ? ? ? ? 80 48 ? ? 40 88 A8 ? ? ? ? 48 89 D8 48 83 C4 ? 5B 5D C3 ? ? ? ? ? ? ? ? ? ? ? ? ? ? 41 57", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction17setTrustedOverlayERKNS_2spINS_14SurfaceControlEEEb
        MethodSignature Transaction_Apply_v12              = { .arm64 = "signature-here", .x86_64 = "55 41 57 41 56 41 55 41 54 53 48 81 EC ? ? ? ? 64 48 8B 04 25 ? ? ? ? 48 89 84 24 ? ? ? ? 8B 87", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction5applyEb
        MethodSignature Transaction_Apply_v13              = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction5applyEbb
        MethodSignature Transaction_SetLayerStack          = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction13setLayerStackERKNS_2spINS_14SurfaceControlEEENS_2ui10LayerStackE
        MethodSignature Transaction_Show                   = { .arm64 = "signature-here", .x86_64 = "41 56 53 50 49 89 F6 48 89 FB E8 ? ? ? ? 80 48 ? ? 80 60", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction4showERKNS_2spINS_14SurfaceControlEEE
        MethodSignature Transaction_Hide                   = { .arm64 = "signature-here", .x86_64 = "41 56 53 50 49 89 F6 48 89 FB E8 ? ? ? ? 80 48 ? ? 80 48", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction4hideERKNS_2spINS_14SurfaceControlEEE
        MethodSignature Transaction_Reparent               = { .arm64 = "signature-here", .x86_64 = "41 57 41 56 41 55 41 54 53 48 83 EC ? 48 89 D3 49 89 F7", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction8reparentERKNS_2spINS_14SurfaceControlEEES6_
        MethodSignature Transaction_SetMatrix              = { .arm64 = "signature-here", .x86_64 = "41 56 53 48 83 EC ? F3 0F 11 5C 24 ? F3 0F 11 54 24 ? F3 0F 11 4C 24 ? F3 0F 11 44 24", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction9setMatrixERKNS_2spINS_14SurfaceControlEEEffff
        MethodSignature Transaction_SetPosition            = { .arm64 = "signature-here", .x86_64 = "41 56 53 50 F3 0F 11 4C 24", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction11setPositionERKNS_2spINS_14SurfaceControlEEEff
        
        // InputWindowInfo variants
        MethodSignature Transaction_SetInputWindowInfo_v10 = { .arm64 = "signature-here", .x86_64 = "55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D4", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction18setInputWindowInfoERKNS_2spINS_14SurfaceControlEEERKNS_3gui10WindowInfoE
        MethodSignature Transaction_SetInputWindowInfo_v16 = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient11Transaction18setInputWindowInfoERKNS_2spINS_14SurfaceControlEEENS2_INS_3gui16WindowInfoHandleEEE

        MethodSignature OpenGlobalTransaction              = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient21openGlobalTransactionEv
        MethodSignature CloseGlobalTransaction             = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android21SurfaceComposerClient22closeGlobalTransactionEb
 
        // SurfaceControl / Surface
        MethodSignature SurfaceControl_GetSurface_v11      = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZNK7android14SurfaceControl10getSurfaceEv
        MethodSignature SurfaceControl_GetSurface_v12      = { .arm64 = "signature-here", .x86_64 = "41 57 41 56 53 48 89 F3 49 89 FF 4C 8D 76 ? 4C 89 F7 E8 ? ? ? ? 48 8B 7B ? 48 85 FF 74 ? ? ? ? 4C 89 FE E8 ? ? ? ? EB ? 4C 89 FF 48 89 DE E8 ? ? ? ? 4C 89 F7 E8 ? ? ? ? 4C 89 F8 5B 41 5E 41 5F C3 ? ? ? ? ? ? ? 41 57", .arm32 = "signature-here" }; // _ZN7android14SurfaceControl10getSurfaceEv
        MethodSignature Surface_DisConnect                 = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android7Surface10disconnectEi
        MethodSignature SurfaceControl_DisConnect          = { .arm64 = "signature-here", .x86_64 = "53 48 83 EC ? 48 89 FB 64 48 8B 04 25 ? ? ? ? 48 89 44 24 ? 48 89 E7 48 89 DE", .arm32 = "signature-here" }; // _ZN7android14SurfaceControl10disconnectEv
        MethodSignature SurfaceControl_GetParentingLayer   = { .arm64 = "signature-here", .x86_64 = "55 48 89 E5 41 56 53 49 89 FE", .arm32 = "signature-here" }; // _ZN7android14SurfaceControl17getParentingLayerEv
        MethodSignature SurfaceControl_SetLayer            = { .arm64 = "signature-here", .x86_64 = "signature-here", .arm32 = "signature-here" }; // _ZN7android14SurfaceControl8setLayerEi OR _ZN7android14SurfaceControl8setLayerEj
    };

    /**
     * @brief Global registry instance.
     */
    inline Registry gRegistry;

} // namespace android::anative_window_creator::signatures
