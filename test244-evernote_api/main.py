from __future__ import division, absolute_import, print_function, unicode_literals

import datetime
import os

from evernote.api.client import EvernoteClient
from evernote.edam.limits.constants import EDAM_USER_NOTES_MAX
from evernote.edam.notestore.ttypes import NoteFilter, NotesMetadataResultSpec
from evernote.edam.userstore.constants import EDAM_VERSION_MAJOR, EDAM_VERSION_MINOR

TIMESTAMP_RESOLUTION = 0.001


def main():
    evernote_token = os.getenv('EVERNOTE_TOKEN')
    client = EvernoteClient(token=evernote_token, sandbox=True)
    validate_version(client)

    note_store = client.get_note_store()
    list_notebooks(note_store)


def list_notebooks(note_store):
    notebooks = note_store.listNotebooks()

    for notebook in notebooks:
        name = notebook.name.decode('utf-8')
        print(u'\n{} ({})'.format(name, notebook.guid))

        list_notes(note_store, notebook)


def list_notes(note_store, notebook):

    filter = NoteFilter(notebookGuid=notebook.guid)
    spec = NotesMetadataResultSpec(
        includeTitle=True,
        includeContentLength=True,
        includeCreated=True,
        includeUpdated=True)

    notes = []
    offset = 0
    remain = EDAM_USER_NOTES_MAX

    while remain > 0:
        notes_list = note_store.findNotesMetadata(filter, offset, remain, spec)

        # See http://dev.evernote.com/doc/reference/NoteStore.html#Struct_NotesMetadataList
        remain = notes_list.totalNotes - (
            notes_list.startIndex + len(notes_list.notes))
        offset += len(notes_list.notes)

        notes.extend(notes_list.notes)

    for note in notes:
        created_time = datetime.datetime.fromtimestamp(
            note.created * TIMESTAMP_RESOLUTION)
        updated_time = datetime.datetime.fromtimestamp(
            note.updated * TIMESTAMP_RESOLUTION)
        title = note.title.decode('utf-8')
        print(u'  {} ({}) ctime={} mtime={}'.format(
            title, note.guid, created_time, updated_time))


def validate_version(client):
    user_store = client.get_user_store()
    if not user_store.checkVersion('', EDAM_VERSION_MAJOR, EDAM_VERSION_MINOR):
        raise Exception('version mismatch')


if __name__ == '__main__':
    main()
