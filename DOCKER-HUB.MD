# Automated Builds on Docker Hub

Let's assume you have a directory in here called `some-project` that holds a `Dockerfile` and `README.MD` and possibly more.

To build these demo's automatically with Docker Hub, we can do the following:

 1. Login to the [Docker Hub](https://hub.docker.com) as an ARPA2 team member
 2. (We'll assume you're a member of the ARPA2 team, otherwise ask)
 3. Click `Repositories` in the top bar, and the `arpa2` project
 4. Click `Create Repository` on the right side
 5. Enter, for `arpa2`, a name such as `some-project` and a brief description
 6. Make it publicly visible
 7. For Build Settings, connect to GitHub
 8. For the project connection, select `arpa2` and `docker-demo`
 9. Customise, possibly by entering and editing later so that:
     - Tag with Source `/^some-project-latest$/` receives Docker Tag `latest` for Dockerfile `/some-project/Dockerfile` or actually just `Dockerfile` relative to `/some-project` but that doesn't show up initially.
     - Tag with Source `/^some-project-([0-9.]+)$/` receives Docker Tag `{\1}` for Dockerfile `/some-project/Dockerfile` or actually just `Dockerfile` relative to `/some-project` but that doesn't show up initially.
10. Click `Create` on the bottom, go to the top for `Builds` and right side for `Configure Automatic Builds` and split the Dockerfile Location `/some-project/Dockerfile` into `Dockerfile` and Build Context `/some-project` which is the reference for files named in the Dockerfile.  This is not possible in the first go, or so it seems.
11. Now close with `Save and Build`

After this, we can go to the repository, and test build the example `some-project`.  When satisfied, we can notify Docker Hub with tags:

```
docker tag --force some-project-latest
docker push --tags --force
```

Now wait a few minutes, and see the positive result in blueish green.

You can inspect the console output to debug any failures.  Do check, the process is error prone, the first time around, because of dependencies and of course you might have entered something wrong.  After the first go, expect a tag update to automatically update your Docker Hub entry.

Very nice is that your `README.MD` will be copied onto the Docker Hub, so good attention to that is invaluable!

Then, the World can download your demo with one of

```
docker pull arpa2/some-project
docker pull arpa2/some-project:latest
```
