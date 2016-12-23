package edu.gatech.cse6242

import java.util.StringTokenizer

import org.apache.spark.SparkContext
import org.apache.spark.SparkConf

object Task2 {
  def main(args: Array[String]) {
    val sc = new SparkContext(new SparkConf().setAppName("Task2"))

    sc.textFile("hdfs://localhost:8020" + args(0))
      .map(line => {
        val st = new StringTokenizer(line)
        st.nextToken()
        (st.nextToken().toInt, st.nextToken().toInt)
      })
      .reduceByKey{case (x, y) => x + y}
      .map(tuple => tuple._1 + "\t" + tuple._2)
      .saveAsTextFile("hdfs://localhost:8020" + args(1))
  }
}
