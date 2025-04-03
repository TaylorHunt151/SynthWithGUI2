#pragma once
//Coding this gave me a headache ;-;

//NOTE: IT WORKS!!

#include <atomic>
#include <vector>
#include <cmath>

class Goodverb { //a reverb that (hopefully) doesn't sound like trash. Work in progress
	//NOTE: The reverb will be based on Dattorro's Plate Reverb, a very popular reverb algorithm designed in 1997.
public:
	std::atomic<bool> on = true; //turns delay on or off. Controlled via checkbox in GUI.
	std::atomic<double> decayTime = 0.9; //Controls how long the reverb is. Range from 0 to 0.999. Controlled via knob.
	std::atomic<double> wetMix = .5; //controls how loud the delayed signal is compared to the unaffected (dry) signal. Ranges from 0 to 1. Controlled via knob/slider in GUI.
	std::atomic<double> dampFreq = 2000; //Controls the lowpass filters in the feedback loop. Range from 30 to 20000, controlled via knob.
	std::atomic<int> preDelay = 300;//Controls how much the signal is delayed before going into the reverb. Range from 0 to 50,000. Controlled via knob.

	std::vector<double> preDelayBuffer;
	int preDelayWritePointer = 0;

	std::vector<double> tap; //cector that stores the delayed signal.
	int writeIndex = 0; //Index for writing to the delay buffer.

	//std::atomic<double> hpCutoff = 100; //this should be changeable via knob/slider (range from 1 to 20,000, default 220. Scaled exponentially)
	//std::atomic<double> hpQ = 1; //this should be changeable (range from 0 to 10) via knob/slider
	std::atomic<double> lpCutoff = 2000; //this should be changeable via knob/slider (range from 1 to 20,000, default 5000. Scaled exponentially)

	std::atomic<double> modFreq = .5;//range from 0.01 to 10
	std::atomic<int> modAmp = 12; //range from 0 to 24
	double modPhase = 0;
	std::vector<int> modBuffer;


	std::vector<double> tempBuffer; //temporary buffer to prevent the original signal from being altered
	std::vector<double> tempBuffer2; //temporary buffer to prevent the original signal from being altered

	double biqCoefs[5] = { 0,0,0,0,0 };//bot changeable. Input filter coefficients
	double dampCoefs[5] = { 0,0,0,0,0, }; //also not changeable. Filter coefficients for the lowpass filter that's in the feedback loop, controlled by the "damping" variable

	//these variables store the filters' input and output registers. 
	std::vector<double> prevLPOut;
    std::vector<double> prevDampOut1;
    std::vector<double> prevDampOut2;

	int dly1 = 210; //apf1 (all pass filter 1) delay time in samples
	std::vector<double> dlyBuffer1;
	int writePointer1 = 0; //Note: the write "pointer" is not actually a pointer. It does not point to a memory address. It is an integer that keeps track of where to write to a circular buffer.
	//Circular buffers are integral parts of delays and reverbs. Essentially, it's a buffer (usually a vector or array) that is constantly being written to and read from, with the read pointer constantly lagging behind the write pointer by a set amount.
	//The buffers round-robin when they reach the end of the buffer.

	int dly2 = 158; //apf2
	std::vector<double> dlyBuffer2;
	int writePointer2 = 0;

	int dly3 = 561;//apf3
	std::vector<double> dlyBuffer3;
	int writePointer3 = 0;

	int dly4 = 410;//apf4
	std::vector<double> dlyBuffer4;
	int writePointer4 = 0;

	int dly5 = 6241;//fixed delay (fDelay) 1
	std::vector<double> dlyBuffer5;
	int writePointer5 = 0;

	int dly6 = 6590;//fDelay 2
	std::vector<double> dlyBuffer6;
	int writePointer6 = 0;

	int dly7 = 4641;//fDelay3
	std::vector<double> dlyBuffer7;
	int writePointer7 = 0;

	int dly8 = 5505;//fDelay4
	std::vector<double> dlyBuffer8;
	int writePointer8 = 0;

	std::vector<int> dly9; //modulated allpass 1
	std::vector<int> dly9Base; //Variable to reset dly9 each cycle
	std::vector<double> dlyBuffer9;
	int writePointer9 = 0;

	std::vector<int> dly10;//modulated allpass 2
	std::vector<int> dly10Base;//variable to reset dly10 each cycle
	std::vector<double> dlyBuffer10;
	int writePointer10 = 0;

	int dly11 = 3931; //apf5
	std::vector<double> dlyBuffer11;
	int writePointer11 = 0;

	int dly12 = 2664;//apf6
	std::vector<double> dlyBuffer12;
	int writePointer12 = 0;


	//these variables are the outputs for the reverb. The reverb functions on a feedback loop containing several filters and delay lines. Rather than having a single output, the output is summed from several points throughout the feedback loop.
	std::vector<double> outA;
	std::vector<double> outB;
	std::vector<double> outC;
	std::vector<double> outD;
	std::vector<double> outE;
	std::vector<double> outF;

	//these variables control the output offset for all the delay lines. It controls where in the feedback loop each output is summed from.
	int offA[3] = { 394, 4401, 3124 };
	int offB[2] = { 2831, 496 };
	int offC[2] = { 2954, 179 };
	int offD[3] = { 2945, 522, 5368 };
	int offE[2] = { 277, 1817 };
	int offF[2] = { 1578, 3956 }; 



	Goodverb() { //Initializes all buffers and vectors to the correct size
		int buffSz = bufferSize.load();

		tempBuffer.resize(buffSz * channelCount, 0);
		tempBuffer2.resize(buffSz * channelCount, 0);

		dly9.resize(channelCount * buffSz, 1343);
		dly10.resize(channelCount * buffSz, 995);

		dlyBuffer1.resize(dly1 * channelCount, 0);
		dlyBuffer2.resize(dly2 * channelCount, 0);
		dlyBuffer3.resize(dly3 * channelCount, 0);
		dlyBuffer4.resize(dly4 * channelCount, 0);
		dlyBuffer5.resize(dly5 * channelCount, 0);
		dlyBuffer6.resize(dly6 * channelCount, 0);
		dlyBuffer7.resize(dly7 * channelCount, 0);
		dlyBuffer8.resize(dly8 * channelCount, 0);
		dlyBuffer9.resize(dly9[0] * channelCount, 0);
		dlyBuffer10.resize(dly10[0] * channelCount, 0);
		dlyBuffer11.resize(dly11 * channelCount, 0);
		dlyBuffer12.resize(dly12 * channelCount, 0);

		outA.resize(buffSz * channelCount * 3, 0);
		outB.resize(buffSz * channelCount * 2, 0);
		outC.resize(buffSz * channelCount * 2, 0);
		outD.resize(buffSz * channelCount * 3, 0);
		outE.resize(buffSz * channelCount * 2, 0);
		outF.resize(buffSz * channelCount * 2, 0);

		modBuffer.resize(channelCount * buffSz, 0);

		dly9Base = dly9;
		dly10Base = dly10;

		preDelayBuffer.resize(50000);

		prevLPOut.resize(2, 0);
		prevDampOut1.resize(2, 0);
		prevDampOut2.resize(2, 0);
	}

	void allpassReverberator(double g, int dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer) {
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int index = i * channelCount + j;

				//calculate read pointer with proper wrapping
				int readPtr = (writePointer - dly + bufferSamples) % bufferSamples;
				int actualReadIdx = readPtr * channelCount + j;

				//get input and delayed samples
				double x = inBuffer[index];
				double y_d = dlyBuffer[actualReadIdx];

				//allpass algorithm (Dattorro's implementation)
				double y = -g * x + y_d;
				inBuffer[index] = y * g + x;

				double feedback = x + g * y_d - g * y;//calculate feedback

				//Write to delay line
				int actualWriteIdx = writePointer * channelCount + j;
				dlyBuffer[actualWriteIdx] = feedback;
			}

			
			writePointer = (writePointer + 1) % bufferSamples;//update write pointer after processing all channels
		}
	}

	std::vector<double> tappedAllpassReverberator(double g, int dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer, int outOffset[2]) {
		std::vector<double> allpassOut(inBuffer.size() * 2, 0);
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				//calculate indices
				int index = i * channelCount + j;
				int outBaseIdx = index * 2; //base index in output buffer

				//calculate circular buffer pointers with channel interleaving
				int readPtr = (writePointer - dly + bufferSamples) % bufferSamples;
				int actualReadIdx = (readPtr * channelCount + j) % dlyBuffer.size();

				//Calculate tap positions - these need to be correct for proper output
				int tapPtr1 = (writePointer - outOffset[0] + bufferSamples) % bufferSamples;
				int tapPtr2 = (writePointer - outOffset[1] + bufferSamples) % bufferSamples;

				int actualTapIdx1 = (tapPtr1 * channelCount + j) % dlyBuffer.size();
				int actualTapIdx2 = (tapPtr2 * channelCount + j) % dlyBuffer.size();

				//extract tap outputs for the final mix
				if (outBaseIdx + 1 < allpassOut.size()) {
					allpassOut[outBaseIdx] = dlyBuffer[actualTapIdx1];
					allpassOut[outBaseIdx + 1] = dlyBuffer[actualTapIdx2];
				}

				//get input and delayed samples
				double x = inBuffer[index];
				double y_d = dlyBuffer[actualReadIdx];

				//allpass algorithm
				double feedforward = -g * x;
				double feedback = g * y_d;
				double y = feedforward + y_d;

				inBuffer[index] = y;//save output

				//Update delay line
				int actualWriteIdx = (writePointer * channelCount + j) % dlyBuffer.size();
				dlyBuffer[actualWriteIdx] = x + feedback;
			}

			writePointer = (writePointer + 1) % bufferSamples;//update write pointer

		}

		return allpassOut;
	}

	std::vector<double> modulatedAllpassReverberator(double g, std::vector<int>& dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer) {
		std::vector<double> outBuffer(inBuffer.size(), 0);
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int index = i * channelCount + j;
				int modIdx = std::min(index, static_cast<int>(dly.size() - 1));

				//get base delay from modulator
				double baseDelay = static_cast<double>(dly[modIdx]);
				int intDelay = static_cast<int>(baseDelay);
				double fracDelay = baseDelay - intDelay;

				//make sure delay is within buffer limits
				intDelay = std::min(std::max(intDelay, 1), bufferSamples - 2);

				//Calculate read positions
				int readPos = (writePointer - intDelay + bufferSamples) % bufferSamples;
				int readPosNext = (readPos - 1 + bufferSamples) % bufferSamples;

				//account for interleaved channels
				int actualReadPos = readPos * channelCount + j;
				int actualReadPosNext = readPosNext * channelCount + j;

				//linear interpolation
				double y_d = dlyBuffer[actualReadPos] * (1 - fracDelay) +
					dlyBuffer[actualReadPosNext] * fracDelay;

				//Standard allpass calculation
				double x = inBuffer[index];
				double y = -g * x + y_d;
				outBuffer[index] = y * g + x;

				//calculate feedback for the delay line
				double feedback = x + g * (y_d - y);

				//Write to delay line
				int actualWriteIdx = writePointer * channelCount + j;
				dlyBuffer[actualWriteIdx] = feedback;
			}

			//update write pointer
			writePointer = (writePointer + 1) % bufferSamples;
		}

		return outBuffer;
	}

	std::vector<double> delayLine(int dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer, int outOffset[3]) {
		std::vector<double> delayOut(inBuffer.size() * 3, 0);
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				//calculate indices
				int index = i * channelCount + j;
				int outBaseIdx = index * 3; //Base index for 3 taps

				//calculate read positions
				int readPtr = (writePointer - dly + bufferSamples) % bufferSamples;
				int actualReadIdx = (readPtr * channelCount + j) % dlyBuffer.size();

				//Calculate tap positions
				int tapPtr1 = (writePointer - outOffset[0] + bufferSamples) % bufferSamples;
				int tapPtr2 = (writePointer - outOffset[1] + bufferSamples) % bufferSamples;
				int tapPtr3 = (writePointer - outOffset[2] + bufferSamples) % bufferSamples;

				int actualTapIdx1 = (tapPtr1 * channelCount + j) % dlyBuffer.size();
				int actualTapIdx2 = (tapPtr2 * channelCount + j) % dlyBuffer.size();
				int actualTapIdx3 = (tapPtr3 * channelCount + j) % dlyBuffer.size();

				//write input sample to delay line
				int actualWriteIdx = (writePointer * channelCount + j) % dlyBuffer.size();
				dlyBuffer[actualWriteIdx] = inBuffer[index];

				inBuffer[index] = dlyBuffer[actualReadIdx];//read from delay line


				//extract tap outputs for final mix
				if (outBaseIdx + 2 < delayOut.size()) {
					delayOut[outBaseIdx] = dlyBuffer[actualTapIdx1];
					delayOut[outBaseIdx + 1] = dlyBuffer[actualTapIdx2];
					delayOut[outBaseIdx + 2] = dlyBuffer[actualTapIdx3];
				}
			}

			//update write pointer
			writePointer = (writePointer + 1) % bufferSamples;
		}

		return delayOut;
	}

	void preDly(int dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer) {
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				//calculate indices
				int index = i * channelCount + j;
				int outBaseIdx = index * 3; //Base index for 3 taps

				//calculate read positions
				int readPtr = (writePointer - dly + bufferSamples) % bufferSamples;
				int actualReadIdx = (readPtr * channelCount + j) % dlyBuffer.size();


				//write input sample to delay line
				int actualWriteIdx = (writePointer * channelCount + j) % dlyBuffer.size();
				dlyBuffer[actualWriteIdx] = inBuffer[index];

				inBuffer[index] = dlyBuffer[actualReadIdx];//read from delay line

			}

			//update write pointer
			writePointer = (writePointer + 1) % bufferSamples;
		}

	}

	std::vector<double> twoTapDelayLine(int dly, std::vector<double>& dlyBuffer, std::vector<double>& inBuffer, int& writePointer, int outOffset[2]) {
		//properly sized output buffer for 2 taps
		std::vector<double> delayOut(inBuffer.size() * 2, 0);
		const int bufferSamples = dlyBuffer.size() / channelCount;

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				//calculate indices
				int index = i * channelCount + j;
				int outBaseIdx = index * 2; //Base index for 2 taps

				//calculate proper read positions with channel interleaving
				int readPtr = (writePointer - dly + bufferSamples) % bufferSamples;
				int actualReadIdx = (readPtr * channelCount + j) % dlyBuffer.size();

				//calculate tap positions
				int tapPtr1 = (writePointer - outOffset[0] + bufferSamples) % bufferSamples;
				int tapPtr2 = (writePointer - outOffset[1] + bufferSamples) % bufferSamples;

				int actualTapIdx1 = (tapPtr1 * channelCount + j) % dlyBuffer.size();
				int actualTapIdx2 = (tapPtr2 * channelCount + j) % dlyBuffer.size();

				//write input sample to delay line
				int actualWriteIdx = (writePointer * channelCount + j) % dlyBuffer.size();
				dlyBuffer[actualWriteIdx] = inBuffer[index];

				//read from delay line
				inBuffer[index] = dlyBuffer[actualReadIdx];

				//extract tap outputs for final mix
				if (outBaseIdx + 1 < delayOut.size()) {
					delayOut[outBaseIdx] = dlyBuffer[actualTapIdx1];
					delayOut[outBaseIdx + 1] = dlyBuffer[actualTapIdx2];
				}
			}

			//update write pointer
			writePointer = (writePointer + 1) % bufferSamples;
		}

		return delayOut;
	}

	std::vector<int> modulator(double freq, int amp, double& phase, std::vector<int> &modBuffer, std::vector<int> modParam) {
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int index = channelCount * i + j;
				modBuffer[index] = floor(amp * (2 * abs(2 * phase - 1) - 1)); //calculates triangle wave values
				phase += (freq / 44100);
				if (phase >= 1) {
					phase = 0;
				}
				modParam[index] += modBuffer[index];
			}
		}
		return modParam;
		
	}

	void onePoleFilter(std::vector<double>& buffer, double cutoff, std::vector<double>& prevOut) { //I'm using a different type of filter here because the biquad filter kept distorting and this one is (supposedly) more stable
		//calculate coefficient
		double a = exp(-2.0 * M_PI * cutoff / 44100.0);

		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int index = i * channelCount + j;

				//One-pole lowpass equation
				double y = buffer[index] * (1.0 - a) + prevOut[j] * a;
				y *= 0.1;//gain staging (making the signal quieter to avoid clipping)

				buffer[index] = y;
				prevOut[j] = y;
			}
		}
	}
	void reverb(double* buffer) {
		//cache atomic values
		const double decay = decayTime.load();
		const double wet = wetMix.load();
		const double dry = 1 - wet;
		const double dampFreqVal = dampFreq.load();

		//Reset dly9 and dly10 to prevent the modulation from accumulating over time
		dly9 = dly9Base;
		dly10 = dly10Base;

		std::copy(buffer, buffer + channelCount * bufferSize, tempBuffer.begin());//copy input to temporary buffer

		onePoleFilter(tempBuffer, lpCutoff, prevLPOut);

		preDly(preDelay, preDelayBuffer, tempBuffer, preDelayWritePointer);

		//input diffusors
		allpassReverberator(0.75, dly1, dlyBuffer1, tempBuffer, writePointer1);
		allpassReverberator(0.75, dly2, dlyBuffer2, tempBuffer, writePointer2);
		allpassReverberator(0.625, dly3, dlyBuffer3, tempBuffer, writePointer3);
		allpassReverberator(0.625, dly4, dlyBuffer4, tempBuffer, writePointer4);

		//Copy pre-diffused signal for tank B feedback mixing
		std::vector<double> inputTankA = tempBuffer;
		std::vector<double> inputTankB(tempBuffer.size(), 0);

		/***********************************************
		********TANK A (left channel processing)********
		************************************************/
		for (int i = 0; i < tempBuffer.size(); i++) {
			tempBuffer[i] += tempBuffer2[i];
		}
		dly9 = modulator(modFreq, modAmp, modPhase, modBuffer, dly9);
		dly10 = modulator(modFreq, modAmp, modPhase, modBuffer, dly10); //Creating modulation signals
		
		tempBuffer = modulatedAllpassReverberator(0.7, dly9, dlyBuffer9, tempBuffer, writePointer9);//modulated all-pass filter

		outA = delayLine(dly5, dlyBuffer5, tempBuffer, writePointer5, offA);//Delay line with taps

		onePoleFilter(tempBuffer, dampFreqVal, prevDampOut1);

		outB = tappedAllpassReverberator(0.5, dly11, dlyBuffer11, tempBuffer, writePointer11, offB);//all-pass filter with taps 


		outC = twoTapDelayLine(dly6, dlyBuffer6, tempBuffer, writePointer6, offC);//delay line with taps


		//add decay feedback to Tank B input
		for (int i = 0; i < tempBuffer.size(); i++) {
			inputTankB[i] = tempBuffer[i] * decay;
		}

		/***********************************************
		*******TANK B (right channel processing)********
		************************************************/

		inputTankB = modulatedAllpassReverberator(0.7, dly10, dlyBuffer10, inputTankB, writePointer10);//modulated all-pass filter for Tank B


		outD = delayLine(dly7, dlyBuffer7, inputTankB, writePointer7, offD);//delay line with taps

		onePoleFilter(inputTankB, dampFreqVal, prevDampOut2);


		outE = tappedAllpassReverberator(0.5, dly12, dlyBuffer12, inputTankB, writePointer12, offE);//All-pass filter with taps


		
		outF = twoTapDelayLine(dly8, dlyBuffer8, inputTankB, writePointer8, offF);//delay line with taps

		
		for (int i = 0; i < inputTankA.size(); i++) {//Add feedback from Tank B to Tank A for next pass
			tempBuffer2[i] = inputTankB[i] * decay;
		}

		//Final mix - combine all taps with appropriate weights
		for (int i = 0; i < bufferSize; i++) {
			for (int j = 0; j < channelCount; j++) {
				int index = i * channelCount + j;



				if (j == 0) { //Left channel mix
					double leftOut = outA[index * 3] + outA[index * 3 + 1] - outB[index * 2] + outC[index * 2] - outD[index * 3] - outE[index * 2] - outF[index * 2];

					buffer[index] = (leftOut * wet * 1000 + buffer[index] * dry);//Mixing the dry and wet signal. NOTE: I'm multiplying the signal by 10 to make it louder, counteracting the attenuation I performed earlier in the signal path.
				}
				else { //Right channel mix
					double rightOut = outD[index * 3 + 1] + outD[index * 3 + 2] - outE[index * 2 + 1] + outF[index * 2 + 1] - outA[index * 3 + 2] - outB[index * 2 + 1] - outC[index * 2 + 1];

					buffer[index] = (rightOut * wet * 1000 + buffer[index] * dry);
				}
			}
		}
	}






};