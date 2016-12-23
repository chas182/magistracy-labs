package edu.gatech.cse6242;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;
import java.util.StringTokenizer;


public class Task1 {


    public static class NodeMapper extends Mapper<Object, Text, IntWritable, IntWritable> {

        private IntWritable weight = new IntWritable();
        private IntWritable targetNode = new IntWritable();

        public void map(Object key, Text value, Context context)
                throws IOException, InterruptedException {
            StringTokenizer st = new StringTokenizer(value.toString());
            // get source node
            st.nextToken();
            // get target node
            targetNode.set(Integer.parseInt(st.nextToken()));
            weight.set(Integer.parseInt(st.nextToken()));
            context.write(targetNode, weight);
        }
    }

    public static class WeightReducer extends Reducer<IntWritable, IntWritable, IntWritable, IntWritable> {
        private IntWritable result = new IntWritable();

        public void reduce(IntWritable key, Iterable<IntWritable> values, Context context)
                throws IOException, InterruptedException {
            int sum = 0;
            for (IntWritable val : values) {
                sum += val.get();
            }
            result.set(sum);
            context.write(key, result);
        }
    }

    public static void main(String[] args) throws Exception {
        Configuration conf = new Configuration();
        Job job = Job.getInstance(conf, "Task 1");

        job.setJarByClass(Task1.class);
        job.setMapperClass(NodeMapper.class);
        job.setCombinerClass(WeightReducer.class);
        job.setReducerClass(WeightReducer.class);
        job.setOutputKeyClass(IntWritable.class);
        job.setOutputValueClass(IntWritable.class);

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }

}
