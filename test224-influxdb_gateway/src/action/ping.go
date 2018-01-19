package action

import (
	"github.com/labstack/echo"
	"github.com/snsinfu/bit4/test224-influxdb_gateway/src/responder"
)

func GetPing(c echo.Context) error {
	dom := getDomain(c)

	pong, err := dom.Ping()
	if err != nil {
		return err
	}
	return responder.GetPing(c, pong)
}
