package rest

import (
	"fmt"
	"net/http"

	"github.com/labstack/echo"
	"github.com/snsinfu/bit4/test223-influxdb_gateway/src/domain"
)

func respondGetPing(c echo.Context, pong domain.Pong) error {
	return c.String(http.StatusOK, fmt.Sprintf("Pong %s\n", pong))
}
