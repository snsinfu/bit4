package main

import (
	"errors"
	"net/http"
	"strconv"
	"time"

	"github.com/labstack/echo"
)

// getChart handles GET "/chart/:market/:pair" request.
func getChart(c echo.Context) error {
	config, err := parseChartConfig(c)
	if err != nil {
		return c.JSON(http.StatusBadRequest, map[string]interface{}{
			"error": err.Error(),
		})
	}

	market := c.Param("market")
	pair := c.Param("pair")

	return c.JSON(http.StatusOK, map[string]interface{}{
		"market": market,
		"pair":   pair,
		"config": config,
	})
}

// chartConfig stores parameters for charting.
type chartConfig struct {
	From time.Time
	To   time.Time
	Bin  time.Duration
}

// parseChartConfig parses query to create a chartConfig.
func parseChartConfig(r *http.Request) (chartConfig, error) {
	var config chartConfig
	var err error
	now := time.Now().UTC()

	config.From, err = parseUnix(r.Query().Get("from"), now.Add(-time.Hour))
	if err != nil {
		return config, err
	}

	config.To, err = parseUnix(r.Query().Get("to"), now)
	if err != nil {
		return config, err
	}

	config.Bin, err = parseDuration(r.Query().Get("bin"), time.Second)
	if err != nil {
		return config, err
	}

	return config, nil
}

// parseUnix parses string as an integer representing Unix time. def is returned
// if the string is empty.
func parseUnix(str string, def time.Time) (time.Time, error) {
	if len(str) == 0 {
		return def, nil
	}
	unix, err := strconv.ParseInt(str, 10, 64)
	if err != nil {
		return def, err
	}
	return time.Unix(unix, 0), nil
}

// parseDuration parses string as an integer representing positive duration. def
// is returned if the string is empty.
func parseDuration(str string, def time.Duration) (time.Duration, error) {
	if len(str) == 0 {
		return def, nil
	}
	sec, err := strconv.ParseInt(str, 10, 64)
	if err != nil {
		return def, err
	}
	if sec <= 0 {
		return def, errors.New("duration must be positive")
	}
	return time.Duration(sec) * time.Second, nil
}
