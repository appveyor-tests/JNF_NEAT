#pragma once
#include "body.h"
#include "training_data.h"
#include <algorithm>

namespace JNF_NEAT {

template <typename InputType, typename Classification, std::size_t ClassificationCount>
class SupervisedTrainigBody : public IBody {
public:
	using Data = TrainingData<InputType, Classification, ClassificationCount>;

	explicit SupervisedTrainigBody(const Data& data) : trainingData{ data } {};

	auto Reset() -> void override { currSet = trainingData.begin(); fitness = 0.0; };
	auto Update(const std::vector<float>& networkOutputs) -> void override;
	auto GetFitness() const -> double override { return fitness; }

	auto HasFinishedTask() const -> bool override { return currSet == trainingData.end(); };
	auto ProvideNetworkWithInputs() const->std::vector<float> override { return currSet->input; };

	auto GetInputCount() const -> std::size_t override { return currSet->input.size(); };
	auto GetOutputCount() const -> std::size_t override { return ClassificationCount; };

	auto GetMaximumFitness() const -> double override { return static_cast<double>(trainingData.GetSize()); };

private:
	const Data& trainingData;
	typename Data::const_iterator currSet = trainingData.begin();
	double fitness = 0.0;
};

template <typename InputType, typename Classification, std::size_t ClassificationCount>
auto SupervisedTrainigBody<InputType, Classification, ClassificationCount>::Update(const std::vector<float>& networkOutputs) -> void {
	auto maxOutput = std::distance(networkOutputs.begin(), std::max_element(networkOutputs.begin(), networkOutputs.end()));
	if (static_cast<size_t>(maxOutput) == static_cast<size_t>(currSet->classification)) {
		fitness += 1.0;
	}
	++currSet;
}

}