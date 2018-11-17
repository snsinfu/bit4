.PHONY: run clean

run: producer-linux consumer-linux
	docker-compose up --abort-on-container-exit

clean:
	rm -f producer-linux consumer-linux
	docker-compose down -v

producer-linux: producer.go
	GOOS=linux go build -o $@ producer.go

consumer-linux: consumer.go
	GOOS=linux go build -o $@ consumer.go
