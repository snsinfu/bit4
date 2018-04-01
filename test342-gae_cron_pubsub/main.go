package main

import (
	"fmt"
	"net/http"
	"time"

	"cloud.google.com/go/pubsub"
	"github.com/go-chi/chi"
	"google.golang.org/api/iterator"
	"google.golang.org/appengine"
	"google.golang.org/appengine/log"
)

// The pubsub library does not work!
// https://github.com/GoogleCloudPlatform/google-cloud-go/issues/702

func main() {
	r := chi.NewRouter()
	r.Get("/topics", handleTopics)
	r.Get("/publish/{topic}", handlePublish)
	http.Handle("/", r)
	appengine.Main()
}

func handleTopics(w http.ResponseWriter, r *http.Request) {
	ctx := appengine.NewContext(r)

	client, err := pubsub.NewClient(ctx, appengine.AppID(ctx))
	if err != nil {
		log.Errorf(ctx, "Failed to create client: %s", err)
		http.Error(w, "", http.StatusInternalServerError)
		return
	}
	defer client.Close()

	w.WriteHeader(http.StatusOK)

	it := client.Topics(ctx)
	for {
		t, err := it.Next()
		if err == iterator.Done {
			break
		}
		if err != nil {
			log.Errorf(ctx, "Failed to list topics: %s", err)
			return
		}
		fmt.Fprintln(w, t)
	}
}

func handlePublish(w http.ResponseWriter, r *http.Request) {
	ctx := appengine.NewContext(r)

	client, err := pubsub.NewClient(ctx, appengine.AppID(ctx))
	if err != nil {
		log.Errorf(ctx, "Failed to create client: %s", err)
		http.Error(w, "", http.StatusInternalServerError)
		return
	}
	defer client.Close()

	name := chi.URLParam(r, "topic")
	topic := client.Topic(name)

	topic.PublishSettings = pubsub.PublishSettings{
		DelayThreshold: 100 * time.Millisecond,
		CountThreshold: 1,
		ByteThreshold:  1e6,
		NumGoroutines:  1,
		Timeout:        1 * time.Second,
	}

	msg := pubsub.Message{
		Data: []byte(name),
	}
	if _, err := topic.Publish(ctx, &msg).Get(ctx); err != nil {
		log.Errorf(ctx, "Failed to publish message: %s", err)
		http.Error(w, "", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusOK)
}
