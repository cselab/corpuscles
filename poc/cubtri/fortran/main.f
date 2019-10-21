C     ALGORITHM 584, COLLECTED ALGORITHMS FROM ACM.
C     ALGORITHM APPEARED IN ACM-TRANS. MATH. SOFTWARE, VOL.8, NO. 2,
C     JUN., 1982, P. 210.
C     PROGRAM KONYN(OUTPUT,TAPE6=OUTPUT)                                MAN   10
C     DRIVER MAIN PROGRAM FOR TESTING SUBROUTINE CUBTRI.  SEVEN         MAN   20
C     INTEGRALS ARE EVALUATED AT TOLERANCES RANGING FROM                MAN   30
C     1E-3 THROUGH 1E-9, TAKING ADVANTAGE OF THE RESTARTABILITY FEATURE.MAN   40
      DIMENSION ACTUAL(7)                                               MAN   50
      REAL ANS, EPS, ERR, T(2,3), W(5000), NINE, RDATA(1)               MAN   60
      INTEGER IDATA(1)                                                  MAN   70
C                                                                       MAN   80
      EXTERNAL F                                                        MAN   90
C                                                                       MAN  100
C     FOR THE FIRST THREE EXAMPLES,T IS THE TRIANGLE CORRESPONDING TO   MAN  110
C     INTEGRAL (0 TO 1)(0 TO X) F(X,Y,IDATA,RDATA) DY DX                MAN  120
C                                                                       MAN  130
      DATA ZERO /0.E0/, ONE /1.E0/, TWO /2.E0/, THREE /3.E0/, SIX /6.E0/MAN  140
      DATA NINE /9.E0/, YTIEN /18.E0/, POINT1 /.1E0/, POINT5 /.5E0/,    MAN  150
     * P002 /1.E-2/                                                     MAN  160
      T(1,1) = ZERO                                                     MAN  170
      T(1,2) = ONE                                                      MAN  180
      T(1,3) = ONE                                                      MAN  190
      T(2,1) = ZERO                                                     MAN  200
      T(2,2) = ZERO                                                     MAN  210
      T(2,3) = ONE                                                      MAN  220
C                                                                       MAN  230
C     SET CORRECT ANSWERS                                               MAN  240
C                                                                       MAN  250
      S = SQRT(THREE)                                                   MAN  260
      ACTUAL(1) = (ALOG(TWO+S))/S                                       MAN  270
      ACTUAL(2) = ACTUAL(1)                                             MAN  280
      ACTUAL(3) = ATAN(ONE)/TWO                                         MAN  290
C       ACTUAL(4) IS THE INTEGRAL (0 TO 1)(0 TO X) OF                   MAN  300
C       EXP(SIN(X)*COS(Y)) DY DX. THIS VALUE, TO 16 DIGITS,IS GIVEN     MAN  310
C       BY A.HAEGEMANS, COMPUTING 19(1977),179-187.                     MAN  320
      ACTUAL(4) = .6918104506612316                                     MAN  330
      ACTUAL(5) = POINT5 - ATAN(ONE) + POINT5*ALOG(TWO)                 MAN  340
      ACTUAL(6) = SIN(THREE)/NINE - SIN(SIX)/YTIEN                      MAN  350
      ACTUAL(7) = TWO/THREE                                             MAN  360
C                                                                       MAN  370
      NFILE = 6                                                         MAN  380
      WRITE (NFILE,99999)                                               MAN  390
      WRITE (NFILE,99998)                                               MAN  400
C                                                                       MAN  410
      DO 40 IEXAMP=1,7                                                  MAN  420
        IDATA(1) = IEXAMP                                               MAN  430
        IF (IEXAMP.NE.4) GO TO 10                                       MAN  440
C                                                                       MAN  450
C       FOR LAST FOUR EXAMPLES,T IS THE TRIANGLE CORRESPONDING TO       MAN  460
C       INTEGRAL (0 TO 1)(0 TO 1-X) F(X,Y,IDATA,RDATA) DY DX            MAN  470
C                                                                       MAN  480
        T(1,2) = ZERO                                                   MAN  490
        T(2,1) = ONE                                                    MAN  500
        T(2,3) = ZERO                                                   MAN  510
   10   EPS = P002                                                      MAN  520
        NCALLS = 0                                                      MAN  530
        MCALLS = 20000                                                  MAN  540
        NW = 5000                                                       MAN  550
        IER = 0                                                         MAN  560
        DO 20 I=1,7                                                     MAN  570
          EPS = EPS*POINT1                                              MAN  580
          IF (IER.GE.4) GO TO 30                                        MAN  590
C                                                                       MAN  600
C          DO NOT GO ON AFTER ROUNDOFF ERRORS HAVE BEEN SIGNALLED       MAN  610
C                                                                       MAN  620
          CALL CUBTRI(F, T, EPS, MCALLS, ANS, ERR, NCALLS, W, NW,       MAN  630
     *     IDATA, RDATA, IER)                                           MAN  640
          ACTU = ABS(ACTUAL(IEXAMP)-ANS)                                MAN  650
          WRITE (NFILE,99997) EPS, ANS, NCALLS, ERR, ACTU, IER          MAN  660
   20   CONTINUE                                                        MAN  670
   30   WRITE (NFILE,99996)                                             MAN  680
   40 CONTINUE                                                          MAN  690
      STOP                                                              MAN  700
C                                                                       MAN  710
99999 FORMAT (1H1, 41X, 6HCUBTRI/42X, 6(1H-)///)                        MAN  720
99998 FORMAT (2X, 8HREQUIRED, 3X, 13HAPPROXIMATION, 5X, 9HNUMBER OF,    MAN  730
     * 5X, 9HESTIMATED, 9X, 6HACTUAL, 12X, 11HTERMINATION/2X, 7HTOLERAN,MAN  740
     * 2HCE, 2X, 11HOF INTEGRAL, 7X, 5HCALLS, 9X, 14HABSOLUTE ERROR,    MAN  750
     * 4X, 14HABSOLUTE ERROR, 4X, 9HINDICATOR/2X, 9(1H-), 2X, 13(1H-),  MAN  760
     * 5X, 9(1H-), 5X, 14(1H-), 4X, 14(1H-), 4X, 11(1H-)/)              MAN  770
99997 FORMAT (3X, 1PE6.0, 3X, 0PF15.12, 3X, I8, 2(10X, 1PE7.1), 5X, I6) MAN  780
99996 FORMAT (//)                                                       MAN  790
      END                                                               MAN  800
      FUNCTION F(X, Y, IDATA, RDATA)                                    F     10
C
      DIMENSION IDATA(1), RDATA(1)
C
      DATA ZERO /0.E0/, ONE /1.E0/, THREE /3.E0/, FOUR /4.E0/, PTWO5
     * /.25E0/
      DATA POINT5 /.5E0/, SIX /6.E0/
      DATA EENE6 /1E-6/, AGTE6 /8E6/, VYFE6 /5E6/
C
      IFUNCT = IDATA(1)
      GO TO (10, 20, 30, 50, 60, 70, 80), IFUNCT
C
   10 F = ONE/SQRT(X*X+THREE*Y*Y)
      RETURN
C
   20 PI = FOUR*ATAN(ONE)
      RAND = EENE6*COS(AMOD(AGTE6*X+VYFE6*Y,PI))
      F = ONE/SQRT(X*X+THREE*Y*Y)*(ONE+RAND)
      RETURN
C
   30 VRAAG = (X-POINT5)**2 + (Y-POINT5)**2
      IF (VRAAG.LE.PTWO5) GO TO 40
      F = ZERO
      RETURN
   40 F = ONE
      RETURN
C
   50 F = EXP(SIN(X)*COS(Y))
      RETURN
C
   60 XX = X*X
      F = XX/(ONE+XX)
      RETURN
C
   70 F = SIN(THREE*X+SIX*Y)
      RETURN
C
   80 F = ONE/SQRT(X+Y)
      RETURN
C
      END
