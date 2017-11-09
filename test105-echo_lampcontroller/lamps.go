package main

import (
	"errors"
	"net/http"
	"strconv"

	"github.com/labstack/echo"
)

type lamp struct {
	ID    int   `json:"id"`
	Red   uint8 `json:"red"   form:"red"`
	Green uint8 `json:"green" form:"green"`
	Blue  uint8 `json:"blue"  form:"blue"`
}

var lamps = []lamp{
	{0, 0, 0, 0},
	{1, 0, 0, 0},
	{2, 0, 0, 0},
	{3, 0, 0, 0},
}

func enumLamps(c echo.Context) error {
	return c.JSON(http.StatusOK, lamps)
}

func getLamp(c echo.Context) error {
	id, err := parseLampID(c.Param("id"))
	if err != nil {
		return c.NoContent(http.StatusNotFound)
	}
	return c.JSON(http.StatusOK, lamps[id])
}

func updateLamp(c echo.Context) error {
	id, err := parseLampID(c.Param("id"))
	if err != nil {
		return c.NoContent(http.StatusNotFound)
	}
	lamp := lamps[id]
	if err := c.Bind(&lamp); err != nil {
		return c.NoContent(http.StatusBadRequest)
	}
	lamp.ID = id
	lamps[id] = lamp
	return c.JSON(http.StatusOK, lamps[id])
}

func parseLampID(str string) (int, error) {
	id, err := strconv.Atoi(str)
	if err != nil {
		return -1, err
	}
	if id < 0 || id >= len(lamps) {
		return id, errors.New("nonexistent ID")
	}
	return id, nil
}
