// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.250303.1

#pragma once
#ifndef WINRT_Windows_Data_Xml_Xsl_2_H
#define WINRT_Windows_Data_Xml_Xsl_2_H
#include "winrt/impl/Windows.Data.Xml.Dom.1.h"
#include "winrt/impl/Windows.Data.Xml.Xsl.1.h"
WINRT_EXPORT namespace winrt::Windows::Data::Xml::Xsl
{
    struct WINRT_IMPL_EMPTY_BASES XsltProcessor : winrt::Windows::Data::Xml::Xsl::IXsltProcessor,
        impl::require<XsltProcessor, winrt::Windows::Data::Xml::Xsl::IXsltProcessor2>
    {
        XsltProcessor(std::nullptr_t) noexcept {}
        XsltProcessor(void* ptr, take_ownership_from_abi_t) noexcept : winrt::Windows::Data::Xml::Xsl::IXsltProcessor(ptr, take_ownership_from_abi) {}
        explicit XsltProcessor(winrt::Windows::Data::Xml::Dom::XmlDocument const& document);
    };
}
#endif
