#include <iostream>
#include <vector>
#include <algorithm>

struct Proposal {
    int proposalNumber;
    std::string value;
};

class Proposer {
public:
    int highestProposalNumber;
    std::string chosenValue;

    Proposal preparePhase() {
        // Pick a unique proposal number higher than any used before
        highestProposalNumber = generateUniqueProposalNumber();

        // Send proposal number to all acceptors during the Prepare phase
        Proposal proposal;
        proposal.proposalNumber = highestProposalNumber;
        return proposal;
    }

    void handlePrepareResponse(const Proposal& response) {
        // Check if any acceptor has accepted a value
        if (response.proposalNumber > highestProposalNumber) {
            highestProposalNumber = response.proposalNumber;
            chosenValue = response.value;
        }
    }

    void acceptPhase() {
        if (!chosenValue.empty()) {
            // Send an Accept request to all acceptors with the chosen value
            // Include the proposal number
            sendAcceptRequests();
        }
    }

    void sendAcceptRequests() {
        // Iterate over acceptors and send Accept request with proposal number and chosen value
        for (auto& acceptor : acceptors) {
            acceptor.acceptProposal(highestProposalNumber, chosenValue);
        }
    }

    void handleAcceptResponse(bool accepted) {
        if (accepted) {
            // Proposal has been accepted by at least one acceptor
            std::cout << "Proposal accepted!" << std::endl;
        }
    }

private:
    int generateUniqueProposalNumber() {
        static int counter = 0;
        return ++counter;
    }

    std::vector<Acceptor> acceptors;  // List of acceptors
};

class Acceptor {
public:
    int highestAcceptedProposalNumber;
    std::string acceptedValue;

    Proposal receivePrepareRequest(int proposalNumber) {
        Proposal response;
        if (proposalNumber > highestAcceptedProposalNumber) {
            response.proposalNumber = highestAcceptedProposalNumber;
            response.value = acceptedValue;
        }
        return response;
    }

    void acceptProposal(int proposalNumber, const std::string& value) {
        if (proposalNumber >= highestAcceptedProposalNumber) {
            highestAcceptedProposalNumber = proposalNumber;
            acceptedValue = value;
        }
        // Send acknowledgment to the proposer
        sendAcceptResponse(true);
    }

    void sendAcceptResponse(bool accepted) {
        // Send acknowledgment to the proposer
        // You can use a communication mechanism of your choice (e.g., network socket)
    }
};

int main() {
    Proposer proposer;
    Acceptor acceptor1, acceptor2, acceptor3;  // Example acceptors

    // Prepare phase
    Proposal proposal = proposer.preparePhase();

    // Send prepare request to acceptors
    Proposal response1 = acceptor1.receivePrepareRequest(proposal.proposalNumber);
    Proposal response2 = acceptor2.receivePrepareRequest(proposal.proposalNumber);
    Proposal response3 = acceptor3.receivePrepareRequest(proposal.proposalNumber);

    // Handle prepare responses
    proposer.handlePrepareResponse(response1);
    proposer.handlePrepareResponse(response2);
    proposer.handlePrepareResponse(response3);

    // Accept phase
    proposer.acceptPhase();

    // Handle accept responses
    proposer.handleAcceptResponse(true);  // Assuming the proposal is accepted

    return 0;
}
