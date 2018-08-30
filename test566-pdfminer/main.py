import re
import signal
import sys

from pdfminer.layout import LTContainer, LTText
from pdfminer.converter import PDFPageAggregator
from pdfminer.pdfparser import PDFParser
from pdfminer.pdfdocument import PDFDocument
from pdfminer.pdfpage import PDFPage
from pdfminer.pdfinterp import PDFResourceManager
from pdfminer.pdfinterp import PDFPageInterpreter
from pdfminer.pdfdevice import PDFDevice


MAX_PAGE = 20

DOI_REGEX = r"doi[:/]?\s*(\d+\.\d+/[a-z0-9-._;()/]+)"
DOIURL_REGEX = r"doi.org/(\d+\.\d+/[a-z0-9-._;()/]+)"
ARXIV_REGEX = r"arxiv:(\d+\.\d+)"

DOI_GARBAGE_REGEXES = [
    r"(\.)$",
    r"(;publishedonline.*)$"
]

TEXT_MOJIBAKES = [
    ("兾", "/")
]


def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

    for path in sys.argv[1:]:
        with open(path, "rb") as file:
            article_id = extract_article_id(file)
            print(f"{article_id}\t{path}")


def extract_article_id(file):
    parser = PDFParser(file)
    document = PDFDocument(parser)

    if not document.is_extractable:
        return None

    resource_manager = PDFResourceManager()
    device = PDFPageAggregator(resource_manager)
    interpreter = PDFPageInterpreter(resource_manager, device)

    for page_index, page in enumerate(PDFPage.create_pages(document)):
        if page_index == MAX_PAGE:
            break

        interpreter.process_page(page)
        page_layout = device.get_result()

        for text in extract_text_blocks(page_layout):
            article_id = find_article_id(text)
            if article_id is not None:
                return cleanup_article_id(article_id)

    return None


def find_article_id(text):
    text = standardize_text(text)

    matches = re.findall(DOI_REGEX, text, re.IGNORECASE)
    if len(matches) > 0:
        return "doi:" + matches[0]

    matches = re.findall(DOIURL_REGEX, text, re.IGNORECASE)
    if len(matches) > 0:
        return "doi:" + matches[0]

    matches = re.findall(ARXIV_REGEX, text, re.IGNORECASE)
    if len(matches) > 0:
        return "arxiv:" + matches[0]

    return None


def standardize_text(text):
    for bake, tru in TEXT_MOJIBAKES:
        text = text.replace(bake, tru)
    return text


def cleanup_article_id(article_id):
    for garbage in DOI_GARBAGE_REGEXES:
        match = re.search(garbage, article_id)
        if match:
            beg, end = match.span()
            article_id = article_id[:beg] + article_id[end:]
    return article_id


def extract_text_blocks(layout):
    text = ""
    prev_item = None

    def is_text(item):
        return isinstance(item, LTText)

    def is_container(item):
        return isinstance(item, LTContainer)

    for item in layout:
        if is_text(item):
            max_distance = 3 * item.width
            if is_text(prev_item) and item.hdistance(prev_item) > max_distance:
                if len(text) > 0:
                    yield text
                    text = ""
            text += item.get_text()
        elif is_container(item):
            for text in extract_text_blocks(item):
                yield text
        else:
            if len(text) != 0:
                yield text
                text = ""
        prev_item = item

    if len(text) != 0:
        yield text


if __name__ == "__main__":
    main()
