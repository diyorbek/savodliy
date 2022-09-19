FROM node:latest
WORKDIR /app
COPY ["package.json", "yarn.lock*", "./"]
RUN apt-get install libstdc++6
RUN yarn install
COPY . .
CMD ["yarn", "start"]