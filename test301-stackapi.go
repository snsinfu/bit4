// go run test301-stackapi > stackoverflow.jsonl
// tail -f stackoverflow.jsonl | jq '[.score, .answer_count, .view_count, .title] | @tsv'

package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"time"
)

const (
	site        = "stackoverflow"
	span        = 24 * time.Hour
	pageSize    = 100
	baseBackoff = 5
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	to := time.Now()
	from := to.Add(-span)

	params := url.Values{}
	params.Set("order", "asc")
	params.Set("sort", "creation")
	params.Set("site", site)
	params.Set("fromdate", strconv.FormatInt(from.Unix(), 10))
	params.Set("todate", strconv.FormatInt(to.Unix(), 10))
	params.Set("pagesize", strconv.Itoa(pageSize))

	encoder := json.NewEncoder(os.Stdout)

	for page := 1; ; page++ {
		params.Set("page", strconv.Itoa(page))

		url := url.URL{
			Scheme:   "https",
			Host:     "api.stackexchange.com",
			Path:     "/2.2/questions",
			RawQuery: params.Encode(),
		}
		log.Println("GET", url.String())

		resp, err := http.Get(url.String())
		if err != nil {
			return err
		}
		defer resp.Body.Close()

		if resp.StatusCode/100 != 2 {
			msg, _ := ioutil.ReadAll(resp.Body)
			return fmt.Errorf("%s %s", resp.Status, string(msg))
		}

		result := QuestionsResult{}
		decoder := json.NewDecoder(resp.Body)
		if err := decoder.Decode(&result); err != nil {
			return err
		}

		for _, q := range result.Items {
			if err := encoder.Encode(&q); err != nil {
				return err
			}
		}

		log.Println("Backoff", result.Backoff, "+", baseBackoff)

		// https://api.stackexchange.com/docs/throttle
		// https://meta.stackexchange.com/q/243773
		time.Sleep(time.Duration(result.Backoff+baseBackoff) * time.Second)

		if !result.HasMore {
			break
		}
	}

	return nil
}

// https://api.stackexchange.com/docs/types/question
type Question struct {
	AcceptedAnswerID   int         `json:"accepted_answer_id"`
	AnswerCount        int         `json:"answer_count"`
	BountyAmount       int         `json:"bounty_amount"`
	BountyClosesDate   int64       `json:"bounty_closes_date"`
	ClosedDate         int64       `json:"closed_date"`
	ClosedReason       string      `json:"closed_reason"`
	CommunityOwnedDate int64       `json:"community_owned_date"`
	CreationDate       int64       `json:"creation_date"`
	IsAnswered         bool        `json:"is_answered"`
	LastActivityDate   int64       `json:"last_activity_date"`
	LastEditDate       int64       `json:"last_edit_date"`
	Link               string      `json:"link"`
	LockedDate         int64       `json:"locked_date"`
	MigratedFrom       interface{} `json:"migrated_from"`
	MigratedTo         interface{} `json:"migrated_to"`
	Owner              interface{} `json:"owner"`
	ProtectedDate      int64       `json:"protected_date"`
	QuestionID         int         `json:"question_id"`
	Score              int         `json:"score"`
	Tags               []string    `json:"tags"`
	Title              string      `json:"title"`
	ViewCount          int         `json:"view_count"`
}

// https://api.stackexchange.com/docs/questions
type QuestionsResult struct {
	Items          []Question `json:"items"`
	HasMore        bool       `json:"has_more"`
	QuotaMax       int        `json:"quota_max"`
	QuotaRemaining int        `json:"quota_remaining"`
	Backoff        int        `json:"backoff"`
}
