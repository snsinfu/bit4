FROM golang:1.12 AS builder
WORKDIR /srv
COPY . .
RUN export CGO_ENABLED=0 \
 && go get -d ./... \
 && go build

FROM scratch
WORKDIR /srv
COPY --from=builder /srv/myapp /srv/myapp

ENV PORT 4000
CMD ["/srv/myapp"]
