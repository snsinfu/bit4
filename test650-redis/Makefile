.PHONY: run clean

run: main-linux
	docker-compose up --abort-on-container-exit

clean:
	rm -f main-linux

main-linux: main.go
	GOOS=linux go build -o $@ .
