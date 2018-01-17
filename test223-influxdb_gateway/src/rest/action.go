package rest

import (
	"github.com/labstack/echo"
	"github.com/snsinfu/bit4/test223-influxdb_gateway/src/domain"
)

// GET /ping
func GetPing(c echo.Context) error {
	dom := getDomain(c)

	pong, err := dom.Ping()
	if err != nil {
		return err
	}
	return respondGetPing(c, pong)
}

func getDomain(c echo.Context) *domain.Domain {
	return c.Get("domain").(*domain.Domain)
}
