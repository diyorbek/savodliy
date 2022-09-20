FROM node:latest
WORKDIR /app
COPY ["package.json", "yarn.lock*", "./"]
COPY . .
RUN yarn install
CMD ["yarn", "start"]