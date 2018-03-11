package main

import (
	"encoding/json"
	"fmt"
	"net/http"
	"net/url"
	"os"
)

func main() {
	if err := run(); err != nil {
		fmt.Fprintln(os.Stderr, "error:", err)
		os.Exit(1)
	}
}

func run() error {
	params := url.Values{}
	params.Set("order", "asc")
	params.Set("sort", "creation")
	params.Set("site", "stackoverflow")
	params.Set("fromdate", "1520640000")
	params.Set("todate", "1520726400")
	params.Set("pagesize", "10")

	url := url.URL{
		Scheme:   "https",
		Host:     "api.stackexchange.com",
		Path:     "/2.2/questions",
		RawQuery: params.Encode(),
	}

	resp, err := http.Get(url.String())
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	if resp.StatusCode >= 300 {
		return fmt.Errorf("%d %s", resp.StatusCode, resp.Status)
	}

	result := QuestionsResult{}
	decoder := json.NewDecoder(resp.Body)
	if err := decoder.Decode(&result); err != nil {
		return err
	}

	for _, q := range result.Items {
		fmt.Printf("%5d  %s\n", q.Score, q.Title)
	}

	return nil
}

// https://api.stackexchange.com/docs/types/question
type Question struct {
	AcceptedAnswerID   int         `json:accepted_answer_id`
	AnswerCount        int         `json:answer_count`
	BountyAmount       int         `json:bounty_amount`
	BountyClosesDate   int64       `json:bounty_closes_date`
	ClosedDate         int64       `json:closed_date`
	ClosedReason       string      `json:closed_reason`
	CommunityOwnedDate int64       `json:community_owned_date`
	CreationDate       int64       `json:creation_date`
	IsAnswered         bool        `json:is_answered`
	LastActivityDate   int64       `json:last_activity_date`
	LastEditDate       int64       `json:last_edit_date`
	Link               string      `json:link`
	LockedDate         int64       `json:locked_date`
	MigratedFrom       interface{} `json:migrated_from`
	MigratedTo         interface{} `json:migrated_to`
	Owner              interface{} `json:owner`
	ProtectedDate      int64       `json:protected_date`
	QuestionID         int         `json:question_id`
	Score              int         `json:score`
	Tags               []string    `json:tags`
	Title              string      `json:title`
	ViewCount          int         `json:view_count`
}

// https://api.stackexchange.com/docs/questions
type QuestionsResult struct {
	Items          []Question `json:items`
	HasMore        bool       `json:has_more`
	QuotaMax       int        `json:quota_max`
	QuotaRemaining int        `json:quota_max`
}
