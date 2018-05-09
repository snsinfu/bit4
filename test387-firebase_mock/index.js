const firebasemock = require('firebase-mock');

const auth = new firebasemock.MockFirebase();
const firestore = new firebasemock.MockFirestore();
const admin = firebasemock.MockFirebaseSdk(null, () => auth, () => firestore);
admin.initializeApp();

const collection = firestore.collection('posts');
const post = collection.doc('aaa');

post.set({ id: 'aaa' })
    .then(() => {
      console.log('ok');
    })
    .catch(e => {
      console.log('error', e);
    });

firestore.flush();

post.get()
    .then(post => {
      console.log('ok', post.data());
    })
    .catch(e => {
      console.log('error', e);
    });

firestore.flush();
