FROM golang:1.12

WORKDIR /srv/src
COPY . .
RUN go get -d ./... \
 && go build \
 && curl -fsSL -o /srv/ftrap https://github.com/snsinfu/ftrap/releases/download/v0.3.0/ftrap \
 && chmod u+x /srv/ftrap

FROM scratch
COPY --from=0 /srv/src/myapp /srv/myapp
COPY --from=0 /srv/ftrap     /srv/ftrap

CMD ["/srv/ftrap", "-f/srv/secrets", "/srv/myapp"]
