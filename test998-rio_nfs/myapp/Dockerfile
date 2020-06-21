FROM golang:1.14
WORKDIR /srv
COPY . .
ENV CGO_ENABLED=0
RUN go build -o main

FROM scratch
COPY --from=0 /srv/main /srv/main
VOLUME /srv/data
CMD ["/srv/main"]
