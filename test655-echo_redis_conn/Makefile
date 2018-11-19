.PHONY: run clean

run: web
	docker-compose up --abort-on-container-exit

clean:
	rm -f web
	docker-compose down -v

web: web.go
	GOOS=linux go build -o $@ web.go
