# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import datetime
import sys
import types
import typing
import uuid as _uuid
from builtins import property as _property

import winrt._winrt
import winrt.system
import winrt.windows.data.xml.dom as windows_data_xml_dom
import winrt.windows.foundation as windows_foundation
import winrt.windows.foundation.collections as windows_foundation_collections
import winrt.windows.security.credentials as windows_security_credentials
import winrt.windows.storage.streams as windows_storage_streams
import winrt.windows.web.syndication as windows_web_syndication

Self = typing.TypeVar('Self')

@typing.final
class AtomPubClient(winrt.system.Object, windows_web_syndication.ISyndicationClient):
    @typing.overload
    def __new__(cls: typing.Type[Self], server_credential: windows_security_credentials.PasswordCredential) -> Self: ...
    @typing.overload
    def __new__(cls: typing.Type[Self]) -> Self: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::CancelAsyncOperations()
    def cancel_async_operations(self) -> None: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Web.Syndication.SyndicationItem,Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::CreateMediaResourceAsync(Windows.Foundation.Uri,System.String,System.String,Windows.Storage.Streams.IInputStream)
    def create_media_resource_async(self, uri: windows_foundation.Uri, media_type: str, description: str, media_stream: windows_storage_streams.IInputStream, /) -> windows_foundation.IAsyncOperationWithProgress[windows_web_syndication.SyndicationItem, windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Web.Syndication.SyndicationItem,Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::CreateResourceAsync(Windows.Foundation.Uri,System.String,Windows.Web.Syndication.SyndicationItem)
    def create_resource_async(self, uri: windows_foundation.Uri, description: str, item: windows_web_syndication.SyndicationItem, /) -> windows_foundation.IAsyncOperationWithProgress[windows_web_syndication.SyndicationItem, windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncActionWithProgress`1<Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::DeleteResourceAsync(Windows.Foundation.Uri)
    def delete_resource_async(self, uri: windows_foundation.Uri, /) -> windows_foundation.IAsyncActionWithProgress[windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncActionWithProgress`1<Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::DeleteResourceItemAsync(Windows.Web.Syndication.SyndicationItem)
    def delete_resource_item_async(self, item: windows_web_syndication.SyndicationItem, /) -> windows_foundation.IAsyncActionWithProgress[windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Web.Syndication.SyndicationFeed,Windows.Web.Syndication.RetrievalProgress> Windows.Web.AtomPub.AtomPubClient::RetrieveFeedAsync(Windows.Foundation.Uri)
    def retrieve_feed_async(self, uri: windows_foundation.Uri, /) -> windows_foundation.IAsyncOperationWithProgress[windows_web_syndication.SyndicationFeed, windows_web_syndication.RetrievalProgress]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Storage.Streams.IInputStream,Windows.Web.Syndication.RetrievalProgress> Windows.Web.AtomPub.AtomPubClient::RetrieveMediaResourceAsync(Windows.Foundation.Uri)
    def retrieve_media_resource_async(self, uri: windows_foundation.Uri, /) -> windows_foundation.IAsyncOperationWithProgress[windows_storage_streams.IInputStream, windows_web_syndication.RetrievalProgress]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Web.Syndication.SyndicationItem,Windows.Web.Syndication.RetrievalProgress> Windows.Web.AtomPub.AtomPubClient::RetrieveResourceAsync(Windows.Foundation.Uri)
    def retrieve_resource_async(self, uri: windows_foundation.Uri, /) -> windows_foundation.IAsyncOperationWithProgress[windows_web_syndication.SyndicationItem, windows_web_syndication.RetrievalProgress]: ...
    # Windows.Foundation.IAsyncOperationWithProgress`2<Windows.Web.AtomPub.ServiceDocument,Windows.Web.Syndication.RetrievalProgress> Windows.Web.AtomPub.AtomPubClient::RetrieveServiceDocumentAsync(Windows.Foundation.Uri)
    def retrieve_service_document_async(self, uri: windows_foundation.Uri, /) -> windows_foundation.IAsyncOperationWithProgress[ServiceDocument, windows_web_syndication.RetrievalProgress]: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::SetRequestHeader(System.String,System.String)
    def set_request_header(self, name: str, value: str, /) -> None: ...
    # Windows.Foundation.IAsyncActionWithProgress`1<Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::UpdateMediaResourceAsync(Windows.Foundation.Uri,System.String,Windows.Storage.Streams.IInputStream)
    def update_media_resource_async(self, uri: windows_foundation.Uri, media_type: str, media_stream: windows_storage_streams.IInputStream, /) -> windows_foundation.IAsyncActionWithProgress[windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncActionWithProgress`1<Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::UpdateResourceAsync(Windows.Foundation.Uri,Windows.Web.Syndication.SyndicationItem)
    def update_resource_async(self, uri: windows_foundation.Uri, item: windows_web_syndication.SyndicationItem, /) -> windows_foundation.IAsyncActionWithProgress[windows_web_syndication.TransferProgress]: ...
    # Windows.Foundation.IAsyncActionWithProgress`1<Windows.Web.Syndication.TransferProgress> Windows.Web.AtomPub.AtomPubClient::UpdateResourceItemAsync(Windows.Web.Syndication.SyndicationItem)
    def update_resource_item_async(self, item: windows_web_syndication.SyndicationItem, /) -> windows_foundation.IAsyncActionWithProgress[windows_web_syndication.TransferProgress]: ...
    # System.UInt32 Windows.Web.AtomPub.AtomPubClient::get_Timeout()
    @_property
    def timeout(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::put_Timeout(System.UInt32)
    @timeout.setter
    def timeout(self, value: winrt.system.UInt32) -> None: ...
    # Windows.Security.Credentials.PasswordCredential Windows.Web.AtomPub.AtomPubClient::get_ServerCredential()
    @_property
    def server_credential(self) -> windows_security_credentials.PasswordCredential: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::put_ServerCredential(Windows.Security.Credentials.PasswordCredential)
    @server_credential.setter
    def server_credential(self, value: windows_security_credentials.PasswordCredential) -> None: ...
    # Windows.Security.Credentials.PasswordCredential Windows.Web.AtomPub.AtomPubClient::get_ProxyCredential()
    @_property
    def proxy_credential(self) -> windows_security_credentials.PasswordCredential: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::put_ProxyCredential(Windows.Security.Credentials.PasswordCredential)
    @proxy_credential.setter
    def proxy_credential(self, value: windows_security_credentials.PasswordCredential) -> None: ...
    # System.UInt32 Windows.Web.AtomPub.AtomPubClient::get_MaxResponseBufferSize()
    @_property
    def max_response_buffer_size(self) -> winrt.system.UInt32: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::put_MaxResponseBufferSize(System.UInt32)
    @max_response_buffer_size.setter
    def max_response_buffer_size(self, value: winrt.system.UInt32) -> None: ...
    # System.Boolean Windows.Web.AtomPub.AtomPubClient::get_BypassCacheOnRetrieve()
    @_property
    def bypass_cache_on_retrieve(self) -> bool: ...
    # System.Void Windows.Web.AtomPub.AtomPubClient::put_BypassCacheOnRetrieve(System.Boolean)
    @bypass_cache_on_retrieve.setter
    def bypass_cache_on_retrieve(self, value: bool) -> None: ...

@typing.final
class ResourceCollection(winrt.system.Object, windows_web_syndication.ISyndicationNode):
    # Windows.Data.Xml.Dom.XmlDocument Windows.Web.AtomPub.ResourceCollection::GetXmlDocument(Windows.Web.Syndication.SyndicationFormat)
    def get_xml_document(self, format: windows_web_syndication.SyndicationFormat, /) -> windows_data_xml_dom.XmlDocument: ...
    # Windows.Foundation.Collections.IVectorView`1<System.String> Windows.Web.AtomPub.ResourceCollection::get_Accepts()
    @_property
    def accepts(self) -> typing.Sequence[str]: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Web.Syndication.SyndicationCategory> Windows.Web.AtomPub.ResourceCollection::get_Categories()
    @_property
    def categories(self) -> typing.Sequence[windows_web_syndication.SyndicationCategory]: ...
    # Windows.Web.Syndication.ISyndicationText Windows.Web.AtomPub.ResourceCollection::get_Title()
    @_property
    def title(self) -> windows_web_syndication.ISyndicationText: ...
    # Windows.Foundation.Uri Windows.Web.AtomPub.ResourceCollection::get_Uri()
    @_property
    def uri(self) -> windows_foundation.Uri: ...
    # Windows.Foundation.Uri Windows.Web.AtomPub.ResourceCollection::get_BaseUri()
    @_property
    def base_uri(self) -> windows_foundation.Uri: ...
    # System.Void Windows.Web.AtomPub.ResourceCollection::put_BaseUri(Windows.Foundation.Uri)
    @base_uri.setter
    def base_uri(self, value: windows_foundation.Uri) -> None: ...
    # System.String Windows.Web.AtomPub.ResourceCollection::get_NodeValue()
    @_property
    def node_value(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ResourceCollection::put_NodeValue(System.String)
    @node_value.setter
    def node_value(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ResourceCollection::get_NodeNamespace()
    @_property
    def node_namespace(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ResourceCollection::put_NodeNamespace(System.String)
    @node_namespace.setter
    def node_namespace(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ResourceCollection::get_NodeName()
    @_property
    def node_name(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ResourceCollection::put_NodeName(System.String)
    @node_name.setter
    def node_name(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ResourceCollection::get_Language()
    @_property
    def language(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ResourceCollection::put_Language(System.String)
    @language.setter
    def language(self, value: str) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.SyndicationAttribute> Windows.Web.AtomPub.ResourceCollection::get_AttributeExtensions()
    @_property
    def attribute_extensions(self) -> typing.MutableSequence[windows_web_syndication.SyndicationAttribute]: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.ISyndicationNode> Windows.Web.AtomPub.ResourceCollection::get_ElementExtensions()
    @_property
    def element_extensions(self) -> typing.MutableSequence[windows_web_syndication.ISyndicationNode]: ...

@typing.final
class ServiceDocument(winrt.system.Object, windows_web_syndication.ISyndicationNode):
    # Windows.Data.Xml.Dom.XmlDocument Windows.Web.AtomPub.ServiceDocument::GetXmlDocument(Windows.Web.Syndication.SyndicationFormat)
    def get_xml_document(self, format: windows_web_syndication.SyndicationFormat, /) -> windows_data_xml_dom.XmlDocument: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Web.AtomPub.Workspace> Windows.Web.AtomPub.ServiceDocument::get_Workspaces()
    @_property
    def workspaces(self) -> typing.Sequence[Workspace]: ...
    # System.String Windows.Web.AtomPub.ServiceDocument::get_NodeValue()
    @_property
    def node_value(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ServiceDocument::put_NodeValue(System.String)
    @node_value.setter
    def node_value(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ServiceDocument::get_NodeNamespace()
    @_property
    def node_namespace(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ServiceDocument::put_NodeNamespace(System.String)
    @node_namespace.setter
    def node_namespace(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ServiceDocument::get_NodeName()
    @_property
    def node_name(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ServiceDocument::put_NodeName(System.String)
    @node_name.setter
    def node_name(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.ServiceDocument::get_Language()
    @_property
    def language(self) -> str: ...
    # System.Void Windows.Web.AtomPub.ServiceDocument::put_Language(System.String)
    @language.setter
    def language(self, value: str) -> None: ...
    # Windows.Foundation.Uri Windows.Web.AtomPub.ServiceDocument::get_BaseUri()
    @_property
    def base_uri(self) -> windows_foundation.Uri: ...
    # System.Void Windows.Web.AtomPub.ServiceDocument::put_BaseUri(Windows.Foundation.Uri)
    @base_uri.setter
    def base_uri(self, value: windows_foundation.Uri) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.SyndicationAttribute> Windows.Web.AtomPub.ServiceDocument::get_AttributeExtensions()
    @_property
    def attribute_extensions(self) -> typing.MutableSequence[windows_web_syndication.SyndicationAttribute]: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.ISyndicationNode> Windows.Web.AtomPub.ServiceDocument::get_ElementExtensions()
    @_property
    def element_extensions(self) -> typing.MutableSequence[windows_web_syndication.ISyndicationNode]: ...

@typing.final
class Workspace(winrt.system.Object, windows_web_syndication.ISyndicationNode):
    # Windows.Data.Xml.Dom.XmlDocument Windows.Web.AtomPub.Workspace::GetXmlDocument(Windows.Web.Syndication.SyndicationFormat)
    def get_xml_document(self, format: windows_web_syndication.SyndicationFormat, /) -> windows_data_xml_dom.XmlDocument: ...
    # Windows.Foundation.Collections.IVectorView`1<Windows.Web.AtomPub.ResourceCollection> Windows.Web.AtomPub.Workspace::get_Collections()
    @_property
    def collections(self) -> typing.Sequence[ResourceCollection]: ...
    # Windows.Web.Syndication.ISyndicationText Windows.Web.AtomPub.Workspace::get_Title()
    @_property
    def title(self) -> windows_web_syndication.ISyndicationText: ...
    # System.String Windows.Web.AtomPub.Workspace::get_NodeValue()
    @_property
    def node_value(self) -> str: ...
    # System.Void Windows.Web.AtomPub.Workspace::put_NodeValue(System.String)
    @node_value.setter
    def node_value(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.Workspace::get_NodeNamespace()
    @_property
    def node_namespace(self) -> str: ...
    # System.Void Windows.Web.AtomPub.Workspace::put_NodeNamespace(System.String)
    @node_namespace.setter
    def node_namespace(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.Workspace::get_NodeName()
    @_property
    def node_name(self) -> str: ...
    # System.Void Windows.Web.AtomPub.Workspace::put_NodeName(System.String)
    @node_name.setter
    def node_name(self, value: str) -> None: ...
    # System.String Windows.Web.AtomPub.Workspace::get_Language()
    @_property
    def language(self) -> str: ...
    # System.Void Windows.Web.AtomPub.Workspace::put_Language(System.String)
    @language.setter
    def language(self, value: str) -> None: ...
    # Windows.Foundation.Uri Windows.Web.AtomPub.Workspace::get_BaseUri()
    @_property
    def base_uri(self) -> windows_foundation.Uri: ...
    # System.Void Windows.Web.AtomPub.Workspace::put_BaseUri(Windows.Foundation.Uri)
    @base_uri.setter
    def base_uri(self, value: windows_foundation.Uri) -> None: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.SyndicationAttribute> Windows.Web.AtomPub.Workspace::get_AttributeExtensions()
    @_property
    def attribute_extensions(self) -> typing.MutableSequence[windows_web_syndication.SyndicationAttribute]: ...
    # Windows.Foundation.Collections.IVector`1<Windows.Web.Syndication.ISyndicationNode> Windows.Web.AtomPub.Workspace::get_ElementExtensions()
    @_property
    def element_extensions(self) -> typing.MutableSequence[windows_web_syndication.ISyndicationNode]: ...

