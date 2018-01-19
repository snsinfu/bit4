package responder

import (
	"fmt"
	"net/http"

	"github.com/labstack/echo"
	"github.com/snsinfu/bit4/test224-influxdb_gateway/src/domain"
)

func GetPing(c echo.Context, pong domain.Pong) error {
	return c.String(http.StatusOK, fmt.Sprintf("Pong %s\n", pong))
}
