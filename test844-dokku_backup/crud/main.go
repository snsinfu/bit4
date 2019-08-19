package main

import (
	"database/sql"
	"log"
	"net/http"
	"os"
	"strconv"
	"time"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
	_ "github.com/lib/pq"
)

func main() {
	db, err := sql.Open("postgres", os.Getenv("DATABASE_URL"))
	if err != nil {
		log.Fatal(err)
	}
	defer db.Close()

	if err := createUsersTable(db); err != nil {
		log.Fatal(err)
	}

	e := echo.New()
	e.Use(middleware.Recover())
	e.Use(middleware.Logger())

	e.GET("/users/:id", func(c echo.Context) error {
		uid, err := strconv.ParseInt(c.Param("id"), 10, 64)
		if err != nil {
			return err
		}
		user, err := getUser(db, uid)
		if err != nil {
			return err
		}
		return c.JSON(http.StatusOK, user)
	})

	e.POST("/users", func(c echo.Context) error {
		uid, err := createUser(db)
		if err != nil {
			return err
		}
		return c.JSON(http.StatusOK, map[string]interface{}{"id": uid})
	})

	e.Logger.Fatal(e.Start(":" + os.Getenv("PORT")))
}

type user struct {
	ID           int64     `json:"id"`
	Name         string    `json:"name"`
	CreationTime time.Time `json:"creation_time"`
}

func createUsersTable(db *sql.DB) error {
	stmt, err := db.Prepare(`
	create table if not exists users (
		id            serial primary key,
		name          text,
		creation_time timestamp
	)
	`)
	if err != nil {
		return err
	}
	defer stmt.Close()

	if _, err := stmt.Exec(); err != nil {
		return err
	}

	return nil
}

func createUser(db *sql.DB) (int64, error) {
	stmt, err := db.Prepare(`
	insert into users (name, creation_time) values ($1, $2) returning id
	`)
	if err != nil {
		return 0, err
	}
	defer stmt.Close()

	var uid int64
	name := "foo"
	ctime := time.Now()
	if err := stmt.QueryRow(name, ctime).Scan(&uid); err != nil {
		return 0, err
	}

	return uid, nil
}

func getUser(db *sql.DB, uid int64) (*user, error) {
	stmt, err := db.Prepare(`select * from users where id = $1`)
	if err != nil {
		return nil, err
	}
	defer stmt.Close()

	var u user
	if err := stmt.QueryRow(uid).Scan(&u.ID, &u.Name, &u.CreationTime); err != nil {
		return nil, err
	}

	return &u, nil
}
