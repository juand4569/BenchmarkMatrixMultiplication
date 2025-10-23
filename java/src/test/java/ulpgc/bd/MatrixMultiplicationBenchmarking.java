package ulpgc.bd;

import org.openjdk.jmh.annotations.*;
import org.openjdk.jmh.infra.Blackhole;

import java.util.Random;
import java.util.concurrent.TimeUnit;

import oshi.SystemInfo;
import java.util.concurrent.atomic.AtomicLong;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@Warmup(iterations = 3, time = 1)
@Measurement(iterations = 5, time = 1)
@Fork(value = 1)
public class MatrixMultiplicationBenchmarking {

    @State(Scope.Benchmark)
    public static class Data {
        @Param({"128","256","512","1024","2048"}) int n;  // ← AÑADIDO 2048

        SystemInfo si;
        long pid;
        volatile boolean running;
        AtomicLong peak = new AtomicLong();
        Thread sampler;

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

            si = new SystemInfo();
            pid = ProcessHandle.current().pid();
            var os = si.getOperatingSystem();
            var proc = os.getProcess((int) pid);
            if (proc != null) {
                long initialRss = proc.getResidentSetSize();
                peak.set(initialRss);
            }

            running = true;
            sampler = new Thread(() -> {
                var os2 = si.getOperatingSystem();
                while (running) {
                    var proc2 = os2.getProcess((int) pid);
                    if (proc2 != null) {
                        long rss = proc2.getResidentSetSize();
                        peak.accumulateAndGet(rss, Math::max);
                    }
                    try { Thread.sleep(200); } catch (InterruptedException ignored) {}
                }
            });
            sampler.setDaemon(true);
            sampler.start();

            try { Thread.sleep(300); } catch (InterruptedException ignored) {}
        }

        @Setup(Level.Iteration)
        public void zeroC() {
            for (int i = 0; i < n; i++) java.util.Arrays.fill(Matrix.c[i], 0.0);
        }

        @TearDown(Level.Trial)
        public void teardown() {
            running = false;
            if (sampler != null) try { sampler.join(300); } catch (InterruptedException ignored) {}
            double peakMB = peak.get() / 1_048_576.0;
            System.out.printf("Matrix size: %dx%d | Peak memory: %.2f MB%n", n, n, peakMB);  // ← MOSTRAR TAMAÑO
        }
    }

    @Benchmark
    public void matrixMultiply(Data d, Blackhole bh) {
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