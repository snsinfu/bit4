class Repository {
  constructor(firebase) {
    this.firebase = firebase;
  }

  list(limit, before) {
    return new Promise((resolve, reject) => {
      const collection = this.firebase.collection('posts');
      const query = collection//.where('date', '<', before)
                              .orderBy('date')
                              .limit(limit);
      query.get()
           .then(querySnapshot => resolve(querySnaoshot.docs))
           .catch(reject);
    });
  }

  put(posts) {
    return new Promise((resolve, reject) => {
      const collection = this.firebase.collection('posts');
      const batch = this.firebase.batch();
      posts.forEach(post => batch.set(collection.doc(post.id), post));
      return batch.commit();
    });
  }
}

module.exports = Repository;
