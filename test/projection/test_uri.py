import unittest
from collections.abc import Sequence
from uuid import UUID

import winrt.windows.foundation as wf


class TestUri(unittest.TestCase):
    def test_activate_uri(self) -> None:
        u = wf.Uri("http://microsoft.com")
        self.assertEqual(u.domain, "microsoft.com")
        self.assertEqual(u.absolute_canonical_uri, "http://microsoft.com/")
        self.assertEqual(u.port, 80)
        self.assertEqual(u.scheme_name, "http")
        self.assertEqual(u.suspicious, False)
        self.assertEqual(u.path, "/")
        self.assertEqual(u.query, "")
        self.assertEqual(u.query_parsed.size, 0)

    def test_activate_uri2(self) -> None:
        u = wf.Uri("http://microsoft.com", "surface/studio")
        self.assertEqual(u.domain, "microsoft.com")
        self.assertEqual(
            u.absolute_canonical_uri, "http://microsoft.com/surface/studio"
        )
        self.assertEqual(u.port, 80)
        self.assertEqual(u.scheme_name, "http")
        self.assertEqual(u.suspicious, False)
        self.assertEqual(u.path, "/surface/studio")
        self.assertEqual(u.query, "")
        self.assertEqual(u.query_parsed.size, 0)

    def test_combine_uri(self) -> None:
        u1 = wf.Uri("http://microsoft.com")
        u = u1.combine_uri("surface/studio")
        self.assertEqual(u.domain, "microsoft.com")
        self.assertEqual(
            u.absolute_canonical_uri, "http://microsoft.com/surface/studio"
        )
        self.assertEqual(u.port, 80)
        self.assertEqual(u.scheme_name, "http")
        self.assertEqual(u.suspicious, False)
        self.assertEqual(u.path, "/surface/studio")
        self.assertEqual(u.query, "")
        self.assertEqual(u.query_parsed.size, 0)

    def test_activate_query_parsed(self) -> None:
        u = wf.Uri("http://microsoft.com?projection=python&platform=windows")
        self.assertEqual(u.query, "?projection=python&platform=windows")

        qp = u.query_parsed
        self.assertEqual(qp.size, 2)

        self.assertEqual(qp.get_first_value_by_name("projection"), "python")
        self.assertEqual(qp.get_first_value_by_name("platform"), "windows")

        e0 = qp.get_at(0)
        self.assertEqual(e0.name, "projection")
        self.assertEqual(e0.value, "python")

        e1 = qp.get_at(1)
        self.assertEqual(e1.name, "platform")
        self.assertEqual(e1.value, "windows")

        t = qp.index_of(e0)
        self.assertTrue(t[0])
        self.assertEqual(t[1], 0)

    def test_query_parsed_is_a_sequence(self) -> None:
        # the type checkers check the call
        def names(entries: Sequence[wf.IWwwFormUrlDecoderEntry]) -> list[str]:
            return [e.name for e in entries]

        qp = wf.Uri(
            "http://microsoft.com?projection=python&platform=windows"
        ).query_parsed

        self.assertEqual(names(qp), ["projection", "platform"])

    def test_runtime_class_name_and_iids(self) -> None:
        uri = wf.Uri("https://example.com")

        self.assertEqual(uri._runtime_class_name_, "Windows.Foundation.Uri")
        # IStringable, one of the interfaces Uri implements
        self.assertIn(UUID("96369f54-8eb6-48f0-abce-c1b211e627c3"), list(uri._iids_))
