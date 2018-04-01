package main

import (
	"encoding/base64"
	"fmt"
	"net/http"

	"golang.org/x/net/context"
	"golang.org/x/oauth2/google"

	"google.golang.org/api/pubsub/v1"
	"google.golang.org/appengine"
	"google.golang.org/appengine/log"

	"github.com/go-chi/chi"
)

func main() {
	r := chi.NewRouter()
	r.Get("/publish/{topic}", handlePublish)
	http.Handle("/", r)
	appengine.Main()
}

func handlePublish(w http.ResponseWriter, r *http.Request) {
	ctx := appengine.NewContext(r)

	project := appengine.AppID(ctx)
	topic := chi.URLParam(r, "topic")
	msg := []byte(topic)

	if err := publish(ctx, project, topic, msg); err != nil {
		log.Errorf(ctx, "Failed to publish a message to topic '%s': %s", topic, err)
		http.Error(w, "", http.StatusInternalServerError)
		return
	}

	w.WriteHeader(http.StatusNoContent)
}

func publish(ctx context.Context, project, topic string, msg []byte) error {
	oauthClient, err := google.DefaultClient(ctx, pubsub.PubsubScope)
	if err != nil {
		return err
	}

	service, err := pubsub.New(oauthClient)
	if err != nil {
		return err
	}

	path := fmt.Sprintf("projects/%s/topics/%s", project, topic)
	message := pubsub.PubsubMessage{
		Data: base64.StdEncoding.EncodeToString(msg),
	}
	request := pubsub.PublishRequest{
		Messages: []*pubsub.PubsubMessage{&message},
	}
	_, err = service.Projects.Topics.Publish(path, &request).Do()
	return err
}
