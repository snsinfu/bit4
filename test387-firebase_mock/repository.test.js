const firebasemock = require('firebase-mock');
const Repository = require('./repository');

describe('repository', () => {
  it('works', () => {
    const firestore = new firebasemock.MockFirestore();
    const repository = new Repository(firestore);

    const t1 = Date.parse('2018-04-22T08:35:19');
    const t2 = Date.parse('2018-04-22T13:50:37');
    const t3 = Date.parse('2018-04-22T23:46:22');

    const inputPosts = [
      { id: 'a111', date: t1 },
      { id: 'a112', date: t2 },
      { id: 'a113', date: t3 }
    ];

    firestore.autoFlush();

    expect(repository.put(inputPosts)).resolves.toBeUndefined();

    expect(repository.list(3, t1))
      .resolves
      .toEqual(inputPosts.slice(1));
  });
});
