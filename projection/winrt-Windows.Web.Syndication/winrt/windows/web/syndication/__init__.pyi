# WARNING: Please don't edit this file. It was generated by Python/WinRT v0.0.0

import enum
import datetime
import sys
import types
import typing

import winrt.system
import winrt.windows.data.xml.dom
import winrt.windows.foundation
import winrt.windows.foundation.collections
import winrt.windows.security.credentials

class SyndicationErrorStatus(enum.IntEnum):
    UNKNOWN = 0
    MISSING_REQUIRED_ELEMENT = 1
    MISSING_REQUIRED_ATTRIBUTE = 2
    INVALID_XML = 3
    UNEXPECTED_CONTENT = 4
    UNSUPPORTED_FORMAT = 5

class SyndicationFormat(enum.IntEnum):
    ATOM10 = 0
    RSS20 = 1
    RSS10 = 2
    RSS092 = 3
    RSS091 = 4
    ATOM03 = 5

class SyndicationTextType(enum.IntEnum):
    TEXT = 0
    HTML = 1
    XHTML = 2

Self = typing.TypeVar('Self')

class RetrievalProgress:
    bytes_retrieved: winrt.system.UInt32
    total_bytes_to_retrieve: winrt.system.UInt32
    def __new__(cls: typing.Type[RetrievalProgress], bytes_retrieved: winrt.system.UInt32, total_bytes_to_retrieve: winrt.system.UInt32) -> RetrievalProgress: ...

class TransferProgress:
    bytes_sent: winrt.system.UInt32
    total_bytes_to_send: winrt.system.UInt32
    bytes_retrieved: winrt.system.UInt32
    total_bytes_to_retrieve: winrt.system.UInt32
    def __new__(cls: typing.Type[TransferProgress], bytes_sent: winrt.system.UInt32, total_bytes_to_send: winrt.system.UInt32, bytes_retrieved: winrt.system.UInt32, total_bytes_to_retrieve: winrt.system.UInt32) -> TransferProgress: ...

class SyndicationAttribute(winrt.system.Object):
    value: str
    namespace: str
    name: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationAttribute: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationAttribute], attribute_name: str, attribute_namespace: str, attribute_value: str) -> SyndicationAttribute:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationAttribute]) -> SyndicationAttribute:...

class SyndicationCategory(winrt.system.Object):
    term: str
    scheme: str
    label: str
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationCategory: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationCategory], term: str) -> SyndicationCategory:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationCategory], term: str, scheme: str, label: str) -> SyndicationCategory:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationCategory]) -> SyndicationCategory:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationClient(winrt.system.Object):
    timeout: winrt.system.UInt32
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    max_response_buffer_size: winrt.system.UInt32
    bypass_cache_on_retrieve: winrt.system.Boolean
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationClient: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationClient], server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]) -> SyndicationClient:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationClient]) -> SyndicationClient:...
    def retrieve_feed_async(self, uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperationWithProgress[SyndicationFeed, RetrievalProgress]: ...
    def set_request_header(self, name: str, value: str, /) -> None: ...

class SyndicationContent(winrt.system.Object):
    source_uri: typing.Optional[winrt.windows.foundation.Uri]
    node_value: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    language: str
    node_name: str
    node_namespace: str
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    text: str
    type: str
    xml: typing.Optional[winrt.windows.data.xml.dom.XmlDocument]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationContent: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationContent], text: str, type: SyndicationTextType) -> SyndicationContent:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationContent], source_uri: typing.Optional[winrt.windows.foundation.Uri]) -> SyndicationContent:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationContent]) -> SyndicationContent:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationError(winrt.system.Object):
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationError: ...
    @staticmethod
    def get_status(hresult: winrt.system.Int32, /) -> SyndicationErrorStatus: ...

class SyndicationFeed(winrt.system.Object):
    title: typing.Optional[ISyndicationText]
    subtitle: typing.Optional[ISyndicationText]
    rights: typing.Optional[ISyndicationText]
    generator: typing.Optional[SyndicationGenerator]
    last_updated_time: datetime.datetime
    image_uri: typing.Optional[winrt.windows.foundation.Uri]
    icon_uri: typing.Optional[winrt.windows.foundation.Uri]
    id: str
    first_uri: typing.Optional[winrt.windows.foundation.Uri]
    items: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationItem]]
    last_uri: typing.Optional[winrt.windows.foundation.Uri]
    links: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationLink]]
    next_uri: typing.Optional[winrt.windows.foundation.Uri]
    previous_uri: typing.Optional[winrt.windows.foundation.Uri]
    categories: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationCategory]]
    source_format: SyndicationFormat
    contributors: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationPerson]]
    authors: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationPerson]]
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationFeed: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationFeed], title: str, subtitle: str, uri: typing.Optional[winrt.windows.foundation.Uri]) -> SyndicationFeed:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationFeed]) -> SyndicationFeed:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...
    def load(self, feed: str, /) -> None: ...
    def load_from_xml(self, feed_document: typing.Optional[winrt.windows.data.xml.dom.XmlDocument], /) -> None: ...

class SyndicationGenerator(winrt.system.Object):
    version: str
    uri: typing.Optional[winrt.windows.foundation.Uri]
    text: str
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationGenerator: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationGenerator], text: str) -> SyndicationGenerator:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationGenerator]) -> SyndicationGenerator:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationItem(winrt.system.Object):
    title: typing.Optional[ISyndicationText]
    source: typing.Optional[SyndicationFeed]
    rights: typing.Optional[ISyndicationText]
    summary: typing.Optional[ISyndicationText]
    published_date: datetime.datetime
    comments_uri: typing.Optional[winrt.windows.foundation.Uri]
    id: str
    last_updated_time: datetime.datetime
    content: typing.Optional[SyndicationContent]
    edit_uri: typing.Optional[winrt.windows.foundation.Uri]
    links: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationLink]]
    authors: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationPerson]]
    categories: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationCategory]]
    contributors: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationPerson]]
    item_uri: typing.Optional[winrt.windows.foundation.Uri]
    e_tag: str
    edit_media_uri: typing.Optional[winrt.windows.foundation.Uri]
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationItem: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationItem], title: str, content: typing.Optional[SyndicationContent], uri: typing.Optional[winrt.windows.foundation.Uri]) -> SyndicationItem:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationItem]) -> SyndicationItem:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...
    def load(self, item: str, /) -> None: ...
    def load_from_xml(self, item_document: typing.Optional[winrt.windows.data.xml.dom.XmlDocument], /) -> None: ...

class SyndicationLink(winrt.system.Object):
    uri: typing.Optional[winrt.windows.foundation.Uri]
    title: str
    resource_language: str
    relationship: str
    media_type: str
    length: winrt.system.UInt32
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationLink: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationLink], uri: typing.Optional[winrt.windows.foundation.Uri]) -> SyndicationLink:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationLink], uri: typing.Optional[winrt.windows.foundation.Uri], relationship: str, title: str, media_type: str, length: winrt.system.UInt32) -> SyndicationLink:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationLink]) -> SyndicationLink:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationNode(winrt.system.Object):
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationNode: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationNode], node_name: str, node_namespace: str, node_value: str) -> SyndicationNode:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationNode]) -> SyndicationNode:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationPerson(winrt.system.Object):
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    uri: typing.Optional[winrt.windows.foundation.Uri]
    name: str
    email: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationPerson: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationPerson], name: str) -> SyndicationPerson:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationPerson], name: str, email: str, uri: typing.Optional[winrt.windows.foundation.Uri]) -> SyndicationPerson:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationPerson]) -> SyndicationPerson:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class SyndicationText(winrt.system.Object):
    node_value: str
    node_namespace: str
    node_name: str
    language: str
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    xml: typing.Optional[winrt.windows.data.xml.dom.XmlDocument]
    type: str
    text: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> SyndicationText: ...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationText], text: str) -> SyndicationText:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationText], text: str, type: SyndicationTextType) -> SyndicationText:...
    @typing.overload
    def __new__(cls: typing.Type[SyndicationText]) -> SyndicationText:...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class ISyndicationClient(winrt.system.Object):
    bypass_cache_on_retrieve: winrt.system.Boolean
    max_response_buffer_size: winrt.system.UInt32
    proxy_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    server_credential: typing.Optional[winrt.windows.security.credentials.PasswordCredential]
    timeout: winrt.system.UInt32
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ISyndicationClient: ...
    def retrieve_feed_async(self, uri: typing.Optional[winrt.windows.foundation.Uri], /) -> winrt.windows.foundation.IAsyncOperationWithProgress[SyndicationFeed, RetrievalProgress]: ...
    def set_request_header(self, name: str, value: str, /) -> None: ...

class ISyndicationNode(winrt.system.Object):
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    language: str
    node_name: str
    node_namespace: str
    node_value: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ISyndicationNode: ...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...

class ISyndicationText(winrt.system.Object):
    text: str
    type: str
    xml: typing.Optional[winrt.windows.data.xml.dom.XmlDocument]
    attribute_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[SyndicationAttribute]]
    base_uri: typing.Optional[winrt.windows.foundation.Uri]
    element_extensions: typing.Optional[winrt.windows.foundation.collections.IVector[ISyndicationNode]]
    language: str
    node_name: str
    node_namespace: str
    node_value: str
    @staticmethod
    def _from(obj: winrt.system.Object, /) -> ISyndicationText: ...
    def get_xml_document(self, format: SyndicationFormat, /) -> typing.Optional[winrt.windows.data.xml.dom.XmlDocument]: ...
