package action

import (
	"github.com/labstack/echo"
	"github.com/snsinfu/bit4/test224-influxdb_gateway/src/domain"
)

const domainKey = "domain"

func Binder(dom *domain.Domain) echo.MiddlewareFunc {
	return func(next echo.HandlerFunc) echo.HandlerFunc {
		return func(c echo.Context) error {
			c.Set(domainKey, dom)
			return next(c)
		}
	}
}

func getDomain(c echo.Context) *domain.Domain {
	return c.Get(domainKey).(*domain.Domain)
}
