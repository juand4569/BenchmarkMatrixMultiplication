package ulpgc.bd;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;

import java.util.Random;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.SingleShotTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(value = 1)
public class MatrixMultiplicationBenchmarking {

    @State(Scope.Benchmark)
    public static class Data {
        @Param({"128","256","512","1024"}) int n;

        @Setup(Level.Trial)
        public void setup() {
            Matrix.n = n;
            Matrix.a = new double[n][n];
            Matrix.b = new double[n][n];
            Matrix.c = new double[n][n];

            Random r = new Random(42);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    Matrix.a[i][j] = r.nextDouble();
                    Matrix.b[i][j] = r.nextDouble();
                }
            }
        }

        @Setup(Level.Iteration)
        public void zeroC() {
            for (int i = 0; i < n; i++) java.util.Arrays.fill(Matrix.c[i], 0.0);
        }
    }

    @Benchmark
    public void classExact(Data d, Blackhole bh) {
        for (int i = 0; i < Matrix.n; i++) {
            for (int j = 0; j < Matrix.n; j++) {
                for (int k = 0; k < Matrix.n; k++) {
                    Matrix.c[i][j] += Matrix.a[i][k] * Matrix.b[k][j];
                }
            }
        }
        double chk = 0.0;
        for (int i = 0; i < Matrix.n; i++) chk += Matrix.c[i][i];
        bh.consume(chk);
    }
}
