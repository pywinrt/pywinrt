// WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

#pragma once

#include "pybase.h"
static_assert(winrt::check_version(PYWINRT_VERSION, "0.0.0"), "Mismatched Py/WinRT headers.");
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Security.Cryptography.Core.h>
#include <winrt/Windows.Storage.Streams.h>

#include <winrt/Windows.Devices.SmartCards.h>

namespace py::proj::Windows::Devices::SmartCards
{
}

namespace py
{
    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardActivationPolicyChangeResult> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroupActivationPolicy> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardAutomaticResponseStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramAlgorithm> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGeneratorOperationStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageConfirmationResponseFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageFormat> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialProtectionMethod> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramPlacementOptions> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyAlgorithm> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyCapabilities> = "I";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardCryptographicKeyAttestationStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardEmulationCategory> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardEmulationType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionDeactivatedReason> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionSource> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardEmulatorEnablementPolicy> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardLaunchBehavior> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardPinCharacterPolicyOption> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardReaderKind> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardReaderStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardStatus> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardTriggerType> = "i";

    template<>
    inline constexpr const char* buffer_format<winrt::Windows::Devices::SmartCards::SmartCardUnlockPromptingBehavior> = "i";


    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardActivationPolicyChangeResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardActivationPolicyChangeResult";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardActivationPolicyChangeResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroupActivationPolicy>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardAppletIdGroupActivationPolicy";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardAppletIdGroupActivationPolicy";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardAutomaticResponseStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardAutomaticResponseStatus";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardAutomaticResponseStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramAlgorithm>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramAlgorithm";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramAlgorithm";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGeneratorOperationStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramGeneratorOperationStatus";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramGeneratorOperationStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageConfirmationResponseFormat>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialPackageConfirmationResponseFormat";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialPackageConfirmationResponseFormat";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageFormat>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialPackageFormat";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialPackageFormat";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialProtectionMethod>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialProtectionMethod";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialProtectionMethod";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialType";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialType";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramPlacementOptions>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramPlacementOptions";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramPlacementOptions";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyAlgorithm>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramStorageKeyAlgorithm";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramStorageKeyAlgorithm";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyCapabilities>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramStorageKeyCapabilities";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramStorageKeyCapabilities";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptographicKeyAttestationStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptographicKeyAttestationStatus";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptographicKeyAttestationStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulationCategory>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulationCategory";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulationCategory";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulationType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulationType";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulationType";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionDeactivatedReason>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorConnectionDeactivatedReason";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorConnectionDeactivatedReason";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionSource>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorConnectionSource";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorConnectionSource";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorEnablementPolicy>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorEnablementPolicy";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorEnablementPolicy";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardLaunchBehavior>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardLaunchBehavior";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardLaunchBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardPinCharacterPolicyOption>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardPinCharacterPolicyOption";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardPinCharacterPolicyOption";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardReaderKind>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardReaderKind";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardReaderKind";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardReaderStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardReaderStatus";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardReaderStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardStatus>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardStatus";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardStatus";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardTriggerType>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardTriggerType";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardTriggerType";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardUnlockPromptingBehavior>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardUnlockPromptingBehavior";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardUnlockPromptingBehavior";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::CardAddedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.CardAddedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "CardAddedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::CardRemovedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.CardRemovedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "CardRemovedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::KnownSmartCardAppletIds>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.KnownSmartCardAppletIds";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "KnownSmartCardAppletIds";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCard>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCard";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCard";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroup>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardAppletIdGroup";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardAppletIdGroup";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroupRegistration>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardAppletIdGroupRegistration";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardAppletIdGroupRegistration";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardAutomaticResponseApdu>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardAutomaticResponseApdu";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardAutomaticResponseApdu";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardChallengeContext>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardChallengeContext";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardChallengeContext";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardConnection>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardConnection";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardConnection";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGenerator>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramGenerator";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramGenerator";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramMaterialCharacteristicsResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramGetAllCryptogramMaterialCharacteristicsResult";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramGetAllCryptogramMaterialCharacteristicsResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramMaterialPackageCharacteristicsResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramGetAllCryptogramMaterialPackageCharacteristicsResult";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramGetAllCryptogramMaterialPackageCharacteristicsResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramStorageKeyCharacteristicsResult>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramGetAllCryptogramStorageKeyCharacteristicsResult";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramGetAllCryptogramStorageKeyCharacteristicsResult";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialCharacteristics>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialCharacteristics";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialCharacteristics";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageCharacteristics>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialPackageCharacteristics";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialPackageCharacteristics";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPossessionProof>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramMaterialPossessionProof";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramMaterialPossessionProof";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramPlacementStep>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramPlacementStep";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramPlacementStep";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyCharacteristics>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramStorageKeyCharacteristics";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramStorageKeyCharacteristics";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyInfo>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardCryptogramStorageKeyInfo";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardCryptogramStorageKeyInfo";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulator>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulator";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulator";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorApduReceivedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorApduReceivedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorApduReceivedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionDeactivatedEventArgs>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorConnectionDeactivatedEventArgs";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorConnectionDeactivatedEventArgs";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionProperties>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardEmulatorConnectionProperties";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardEmulatorConnectionProperties";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardPinPolicy>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardPinPolicy";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardPinPolicy";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardPinResetDeferral>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardPinResetDeferral";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardPinResetDeferral";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardPinResetRequest>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardPinResetRequest";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardPinResetRequest";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardProvisioning>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardProvisioning";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardProvisioning";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardReader>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardReader";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardReader";
    };

    template<>
    struct py_type<winrt::Windows::Devices::SmartCards::SmartCardTriggerDetails>
    {
        static constexpr std::string_view qualified_name = "winrt.windows.devices.smartcards.SmartCardTriggerDetails";
        static constexpr const char* module_name = "winrt.windows.devices.smartcards";
        static constexpr const char* type_name = "SmartCardTriggerDetails";
    };
}

#if __has_include("py.Windows.Foundation.h")
#include "py.Windows.Foundation.h"
#endif

#if __has_include("py.Windows.Foundation.Collections.h")
#include "py.Windows.Foundation.Collections.h"
#endif

#if __has_include("py.Windows.Security.Cryptography.Core.h")
#include "py.Windows.Security.Cryptography.Core.h"
#endif

#if __has_include("py.Windows.Storage.Streams.h")
#include "py.Windows.Storage.Streams.h"
#endif

namespace py::impl::Windows::Devices::SmartCards
{
    struct SmartCardPinResetHandler
    {
        static winrt::Windows::Devices::SmartCards::SmartCardPinResetHandler get(PyObject* callable)
        {
            py::delegate_callable _delegate{ callable };

            return [delegate = std::move(_delegate)](winrt::Windows::Devices::SmartCards::SmartCardProvisioning const& param0, winrt::Windows::Devices::SmartCards::SmartCardPinResetRequest const& param1)
            {
                auto gil = py::ensure_gil();

                try
                {
                    py::pyobj_handle py_param0{py::convert(param0)};
                    if (!py_param0)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle py_param1{py::convert(param1)};
                    if (!py_param1)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle args{PyTuple_Pack(2, py_param0.get(), py_param1.get())};
                    if (!args)
                    {
                        throw python_exception();
                    }

                    py::pyobj_handle return_value{PyObject_CallObject(delegate.callable(), args.get())};
                    if (!return_value)
                    {
                        throw python_exception();
                    }
                }
                catch (python_exception)
                {
                    py::write_unraisable_and_throw();
                }
            };
        };
    };
}

namespace py::wrapper::Windows::Devices::SmartCards
{
    using CardAddedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::CardAddedEventArgs>;
    using CardRemovedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::CardRemovedEventArgs>;
    using KnownSmartCardAppletIds = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::KnownSmartCardAppletIds>;
    using SmartCard = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCard>;
    using SmartCardAppletIdGroup = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroup>;
    using SmartCardAppletIdGroupRegistration = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardAppletIdGroupRegistration>;
    using SmartCardAutomaticResponseApdu = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardAutomaticResponseApdu>;
    using SmartCardChallengeContext = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardChallengeContext>;
    using SmartCardConnection = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardConnection>;
    using SmartCardCryptogramGenerator = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGenerator>;
    using SmartCardCryptogramGetAllCryptogramMaterialCharacteristicsResult = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramMaterialCharacteristicsResult>;
    using SmartCardCryptogramGetAllCryptogramMaterialPackageCharacteristicsResult = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramMaterialPackageCharacteristicsResult>;
    using SmartCardCryptogramGetAllCryptogramStorageKeyCharacteristicsResult = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramGetAllCryptogramStorageKeyCharacteristicsResult>;
    using SmartCardCryptogramMaterialCharacteristics = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialCharacteristics>;
    using SmartCardCryptogramMaterialPackageCharacteristics = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPackageCharacteristics>;
    using SmartCardCryptogramMaterialPossessionProof = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramMaterialPossessionProof>;
    using SmartCardCryptogramPlacementStep = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramPlacementStep>;
    using SmartCardCryptogramStorageKeyCharacteristics = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyCharacteristics>;
    using SmartCardCryptogramStorageKeyInfo = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardCryptogramStorageKeyInfo>;
    using SmartCardEmulator = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardEmulator>;
    using SmartCardEmulatorApduReceivedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardEmulatorApduReceivedEventArgs>;
    using SmartCardEmulatorConnectionDeactivatedEventArgs = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionDeactivatedEventArgs>;
    using SmartCardEmulatorConnectionProperties = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardEmulatorConnectionProperties>;
    using SmartCardPinPolicy = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardPinPolicy>;
    using SmartCardPinResetDeferral = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardPinResetDeferral>;
    using SmartCardPinResetRequest = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardPinResetRequest>;
    using SmartCardProvisioning = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardProvisioning>;
    using SmartCardReader = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardReader>;
    using SmartCardTriggerDetails = py::winrt_wrapper<winrt::Windows::Devices::SmartCards::SmartCardTriggerDetails>;
}

namespace py
{
    template <>
    struct delegate_python_type<winrt::Windows::Devices::SmartCards::SmartCardPinResetHandler>
    {
        using type = py::impl::Windows::Devices::SmartCards::SmartCardPinResetHandler;
    };

}
