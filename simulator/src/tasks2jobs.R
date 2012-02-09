#!/usr/bin/Rscript

arquivo <- read.table("workload-tasks.txt")
arquivo <- cbind(arquivo, V6=sum(arquivo$V5))
arquivo2 <- aggregate(arquivo, list(arquivo$V2, arquivo$V3), max)
arquivo3 <- arquivo2[,4:8]
write.table(arquivo3, file="workload-jobs.txt", col.names=FALSE, row.names=FALSE)