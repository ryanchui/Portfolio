# SID: 24061277
# Class: Stat133
# title: "Project"
# output: html_document

readData = function(fileName,dateFormat="%m/%d/%y"){
  stockData = read.csv(fileName, 
                       colClasses=c("character", "NULL","NULL", "NULL","NULL","NULL", "numeric"))
  stockData$Date = as.Date(stockData$Date, dateFormat)
  stockData = stockData[order(stockData$Date), ]
  return(stockData)
}

combine2stocks = function(stockA, stockB){
  stockA = readData (stockA)
  stockB = readData (stockB)
  stockA = stockA[stockA$Date %in% stockB$Date, ]
  stockB = stockB [stockB$Date %in% stockA$Date, ]
  Date = stockA$Date
  Adj.Close.A = stockA$Adj.Close
  Adj.Close.B = stockB$Adj.Close
  ratio = stockA$Adj.Close/stockB$Adj.Close
  return(data.frame(Date, Adj.Close.A, Adj.Close.B, ratio))
}
ford = readData ("http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv")
gm = readData ("http://www.stat.berkeley.edu/~nolan/data/stocks/gm.csv")
org = read.csv("http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv")
intc= read.csv("http://www.stat.berkeley.edu/~nolan/data/stocks/intc.csv")
test = combine2stocks("http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv","http://www.stat.berkeley.edu/~nolan/data/stocks/gm.csv")
test2 = combine2stocks ("http://www.stat.berkeley.edu/~nolan/data/stocks/intc.csv", "http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv")


intc.url = "http://www.stat.berkeley.edu/~nolan/data/stocks/intc.csv"
ford.url = "http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv"
gm.csv = "http://www.stat.berkeley.edu/~nolan/data/stocks/gm.csv"

plotRatio = function(ratio, date = seq(along = ratio),k = 1,...){
  plot(ratio ~ date, xlab = "Date", ylab = "Ratio", type = 'l',...)
  abline(h = mean(ratio), col = 'green', lty = 2)
  abline(h = mean(ratio) + k * sd(ratio), col = 'red', lty = 2)
  abline(h = mean(ratio) - k * sd(ratio), col = "red", lty = 2)
  return(plot)
}
test3 = plotRatio (ratio = combine2stocks(ford.url,intc.url)$ratio, date = combine2stocks(ford.url,intc.url)$Date)


# Function 4:
showPosition = function(pos, ratios, col = 2:3, radius = 100,...){
  points(ratios~pos, cex = radius, pch = 1, col = col,...)
  return(plot)
}

# Function 5
findNextPosition = function (ratio, startDay = 1, k=1, m = mean (ratio), s = sd (ratio)) {
  openings = which(ratio <= m - k*s | ratio >= (m + k*s))
  next1 = openings [openings >= startDay][1] 
  if(is.na (next1)) {
    return (integer())
  }   
  if(ratio[next1] >= m + k*s) {
    close = which(ratio < m)
    firstclose = close [close > next1][1] 
  } else {
    if (ratio [next1] <= m + k*s){
      close = which(ratio > m)
      firstclose = close [close > next1][1]
    }
  }
  if(is.na (firstclose)) {
    firstclose = length (ratio)
  }
  return (c(next1, firstclose))
}

findNextPosition (ratio = test2[ ,"ratio"], k =2)

# Function 6: getPositions() 
getPosition = function (ratio, k = 1, start= 1) {
  i = start
  num = k
  open = c(findNextPosition (ratio, k = num, i)[1])
  close = c(findNextPosition (ratio, k = num, i)[2])
  i = max (close)
  while (all (is.na (close)==F) & i <= length (ratio)){
    open = c(open, findNextPosition (ratio, k = num, i)[1])
    close = c(close, findNextPosition (ratio, k =num, i)[2])
    i = max (close)
  }
  return (list(open[-length(open)], close[-length (close)]))
}

getPosition (ratio = test2[ ,"ratio"], k =2)

# positionProfit()
getPositions = function(ratio, k = 1){
  m = mean(ratio)
  s = sd(ratio)
  pos = findNextPosition(ratio,startDay = 1, k = k, m = m, s = s)
  pos2 = data.frame(pos)
  while(!((length(pos) == 0) | (pos[2] == length(ratio)))){
    pos = findNextPosition(ratio,startDay = pos[2], k = k, m = m, s = s)
    if(length(pos) == 0){
      break
    }
    pos2 = data.frame(pos2,pos)
  }
  poslist= as.list(pos2)
  return(poslist)
}
test4= combine2stocks ("http://www.stat.berkeley.edu/~nolan/data/stocks/gm.csv", "http://www.stat.berkeley.edu/~nolan/data/stocks/f.csv")
positionProfit (pos = c (100, 50), stockPriceA = test4[ , "Adj.Close.A"], stockPriceB = test4 [, "Adj.Close.B"] )

# Function 8: getProfit.K() 
getProfit.K = function (x, y, m = mean(x/y), s = sd(x/y), k){
  positions = getPositions(x/y, k=k)
  profit = numeric(1)
  start = 1
  while(start < length(positions)) {
    profit = c(profit, positionProfit(pos = positions[[start]], x[positions[[start]]], y[positions[[start]]], m))
    start = start + 1
  }
  return(sum(profit))
}
sum (getProfit.K (x = test$Stock1, y = test$Stock2, k = 2))

# Function 9 
getBest.K = function (x, y, k.min, k.max, numK, m = mean (x/y), s= sd (x/y)){
  val = k.min
  sums = as.numeric()
  for(val in val <= k.max) {
    sums = c (sums, sum(getProfit.K (x, y, k = val)))
    val = val + (k.max - k.min)/(numK-1)
  }
  return ((which.max (sums)-1) * (k.max-k.min)/(numK-1) + k.min)
}
getBest.K (x = test$Stock1, y = test$Stock2, k.min = 1, k.max = 2, numK=3)

# Function 10
stockSim = function(n = 300, rho = 0.99, psi = 0, sigma = rep(1, 2),
                    beta0 = rep(100, 2), beta1 = rep(0, 2),
                    epsilon = matrix(rnorm(2*n, sd = sigma),
                                     nrow = n, byrow = TRUE)){
  X = matrix(rep(0,2*n), nrow = n, byrow = TRUE)
  op.x = matrix(c(rho, psi*(1 - rho), psi*(1 - rho), rho), nrow = 2, byrow = TRUE)
  X[1,] = epsilon[1,]
  for(i in 2:n){
    X[i,] =  (X[i-1,] %*% op.x) + epsilon[i,]
  }
  Y = rep(beta0, each = n) + rep(beta1, each = n)*(1:n) + X
  return(Y)
}

# Function 11
runSim = function(rho, psi, beta0 = c(100, 100), beta1 = c(0, 0),
                  sigma = c(1, 1), n = 4000){
  stock = stockSim(n = n, rho = rho, psi = psi, beta0 = beta0, beta1 = beta1, sigma = sigma)
  test = stock[(floor(n/2) + 1):n,]
  stockA1 = stock[1:(n/2),1]
  stockB1 = stock[1:(n/2),2]
  k.max = (max(abs(stockA1/stockB1 - mean(stockA1/stockB1)))/sd(stockA1/stockB1)) - 0.01
  m = mean(stockA1/stockB1)
  s = sd(stockA1/stockB1)
  k.star = getBest.K(stockA1, stockB1, k.max = k.max, 
                     k.min = .1, numK = 100, m = m ,s = s)
  profit = getProfit.K(test[,1], test[,2], k = k.star, m = m, s = s)
  return(profit)
}

# Function 12
RHOS = c(0.1, 0.1, 0.2, 0.4, 0.7,0.75)
PSIS = c(0.5, 1, 1.5, 2, 2.5,4)
BETA0 = list(c(100,100))
BETA1 = list(c(0,0.05),c(0,0),c(.05,.05),c(.05,0))
SIGMAS = list(c(.5,.5),c(1,1))

myParameters = expand.grid(rho = RHOS, 
                           psi = PSIS, 
                           beta0 = BETA0,
                           beta1 = BETA1, 
                           sigma = SIGMAS)

myMatrix = matrix(nrow = nrow(myParameters), ncol = 5)

simProfitDist =
  function(..., B = 5)
    sapply(1:B, function(i, ...) runSim(...), ...)
for (i in 1:nrow(myParameters)){
  prof = simProfitDist(rho = myParameters[i, "rho"], psi = myParameters[i, "psi"], sigma = myParameters[i, "sigma"][[1]], 
                       beta0 = myParameters[i, "beta0"][[1]], beta1 = myParameters[i,"beta1"][[1]], B = 5)
  myMatrix[i,] = prof
}

save(simResult, file = "simResult.rda")