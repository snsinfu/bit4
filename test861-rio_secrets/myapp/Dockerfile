FROM golang:1.12

WORKDIR /srv/src
COPY . .
RUN go get -d ./... && go build

FROM scratch
COPY --from=0 /srv/src/myapp /srv/myapp

CMD ["/srv/myapp"]
