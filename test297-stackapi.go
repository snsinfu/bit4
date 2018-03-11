package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"strconv"
	"time"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	site := "stackoverflow"
	span := 24 * time.Hour

	to := time.Now()
	from := to.Add(-span)
	questions, err := QueryQuestions(site, from, to)
	if err != nil {
		return err
	}

	for _, q := range questions {
		fmt.Printf(
			"%5d %2d  %s  %s\n",
			q.Score,
			q.AnswerCount,
			time.Unix(q.CreationDate, 0).Format("15:04:05"),
			q.Title,
		)
	}

	return nil
}

func QueryQuestions(site string, from, to time.Time) ([]Question, error) {
	const pageSize = 100

	params := url.Values{}
	params.Set("order", "asc")
	params.Set("sort", "creation")
	params.Set("site", site)
	params.Set("fromdate", strconv.FormatInt(from.Unix(), 10))
	params.Set("todate", strconv.FormatInt(to.Unix(), 10))
	params.Set("pagesize", strconv.Itoa(pageSize))

	questions := []Question{}

	for page := 1; ; page++ {
		params.Set("page", strconv.Itoa(page))

		url := url.URL{
			Scheme:   "https",
			Host:     "api.stackexchange.com",
			Path:     "/2.2/questions",
			RawQuery: params.Encode(),
		}
		fmt.Println("GET", url.String())

		resp, err := http.Get(url.String())
		if err != nil {
			return questions, err
		}
		defer resp.Body.Close()

		if resp.StatusCode >= 300 {
			msg, _ := ioutil.ReadAll(resp.Body)
			return questions, fmt.Errorf("%s %s", resp.Status, string(msg))
		}

		result := QuestionsResult{}
		decoder := json.NewDecoder(resp.Body)
		if err := decoder.Decode(&result); err != nil {
			return questions, err
		}

		questions = append(questions, result.Items...)

		if !result.HasMore {
			break
		}

		// https://api.stackexchange.com/docs/throttle
		// https://meta.stackexchange.com/q/243773
		time.Sleep(time.Duration(result.Backoff) * time.Second)
	}

	return questions, nil
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
