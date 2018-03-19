package main

import (
	"fmt"

	"github.com/aws/aws-lambda-go/lambda"
)

type Event struct {
	HttpMethod            string     `json:"httpMethod"`
	QueryStringParameters Parameters `json:"queryStringParameters"`
}

type Parameters struct {
	Name string `json:"name"`
}

type Response struct {
	StatusCode      int               `json:"statusCode"`
	Headers         map[string]string `json:"headers"`
	Body            string            `json:"body"`
	IsBase64Encoded bool              `json:"isBase64Encoded"`
}

func HandleRequest(event Event) (Response, error) {
	resp := Response{
		IsBase64Encoded: false,
		StatusCode:      200,
		Headers:         map[string]string{"Content-Type": "text/plain; charset=utf-8"},
		Body:            fmt.Sprintf("Hello, %s!", event.QueryStringParameters.Name),
	}
	return resp, nil
}

func main() {
	lambda.Start(HandleRequest)
}
