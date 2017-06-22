// @flow

type Article = {
  title: ?string,
  uri: string
}

function queryArticle (uri: string): Article {
  return {
    title: null,
    uri: uri
  }
}

console.log(queryArticle('doi:10.1000/182'))
