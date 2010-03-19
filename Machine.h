/*
 	Ray
    Copyright (C) 2010  Sébastien Boisvert

	http://DeNovoAssembler.SourceForge.Net/

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You have received a copy of the GNU General Public License
    along with this program (gpl-3.0.txt).  
	see <http://www.gnu.org/licenses/>

*/


#ifndef _Machine
#define _Machine

#include<common_functions.h>
#include<map>
#include<vector>
#include<Vertex.h>
#include<SplayTree.h>
#include<Message.h>
#include<time.h>
#include<SplayTreeIterator.h>
#include<set>
#include<Read.h>
#include<Parameters.h>
#include<MyAllocator.h>
#include<mpi.h>
#include<stack>

/*
 * put some members in this class or else g++ don't like it. it makes otherwise the program segfault!
 */
class BubbleData{
public:
	vector<vector<VERTEX_TYPE> > m_BUBBLE_visitedVertices;
	bool m_doChoice_bubbles_Detected;
	bool m_doChoice_bubbles_Initiated;
	vector<vector<int> > m_BUBBLE_visitedVerticesDepths;
	vector<map<VERTEX_TYPE,int> > m_coverages;
};

class DepthFirstSearchData{
public:
	// depth first search
	bool m_doChoice_tips_Initiated;
	bool m_doChoice_tips_dfs_done;
	int m_depthFirstSearch_maxDepth;
	stack<int> m_depthFirstSearchDepths;
	int m_doChoice_tips_i;
	vector<VERTEX_TYPE> m_doChoice_tips_newEdges;
	bool m_doChoice_tips_dfs_initiated;
	set<VERTEX_TYPE> m_depthFirstSearchVisitedVertices;
	stack<VERTEX_TYPE> m_depthFirstSearchVerticesToVisit;
	vector<VERTEX_TYPE> m_depthFirstSearchVisitedVertices_vector;
	vector<int> m_depthFirstSearchVisitedVertices_depths;
	map<VERTEX_TYPE,int> m_coverages;
};

using namespace std;


class Machine{
	int m_argc;
	char**m_argv;
	int m_wordSize;
	int m_last_value;
	time_t m_lastTime;
	bool m_mode_send_outgoing_edges;
	int m_mode_send_edge_sequence_id;
	int m_mode_send_edge_sequence_id_position;
	int m_rank;
	int m_size;
	bool m_reverseComplementEdge;
	int m_totalLetters;
	bool m_alive;
	bool m_welcomeStep;
	bool m_loadSequenceStep;
	char*m_inputFile;
	int m_sequence_ready_machines;
	bool m_messageSentForVerticesDistribution;

	// speed calibration to make OpenMPI handle the communication in its shared memory.
	int m_calibrationStart;
	bool m_speedLimitIsOn;
	int m_calibration_numberOfMessagesSent;
	bool m_calibrationAskedCalibration;
	int m_calibration_MaxSpeed;
	bool m_calibrationIsDone;

	// clearing
	int m_CLEAR_n;

	int m_readyToSeed;
	bool m_showMessages;
	bool m_mode_send_ingoing_edges;

	int m_mode;
	int m_master_mode;

	// TODO: merge all m_mode_* variables with m_mode and m_master_mode.
	bool m_startEdgeDistribution;
	bool m_mode_AttachSequences;

	// Counters.
	int m_numberOfMachinesReadyForEdgesDistribution;
	int m_ranksDoneAttachingReads;
	int m_numberOfMachinesReadyToSendDistribution;
	int m_vertices_sent;
	int m_numberOfRanksDoneSeeding;
	int m_numberOfRanksGone;
	map<int,VERTEX_TYPE> m_distributionOfCoverage;

	int m_machineRank;

	// SEEDING
	SplayTreeIterator<VERTEX_TYPE,Vertex>*m_SEEDING_iterator;
	SplayNode<VERTEX_TYPE,Vertex>*m_SEEDING_node;
	bool m_SEEDING_edgesReceived;
	int m_SEEDING_numberOfOutgoingEdgesWithSeedCoverage;
	VERTEX_TYPE m_SEEDING_currentChildVertex;
	VERTEX_TYPE m_SEEDING_currentParentVertex;
	VERTEX_TYPE m_SEEDING_receivedKey;
	bool m_SEEDING_vertexKeyAndCoverageReceived;
	vector<VERTEX_TYPE> m_SEEDING_nodes;
	VERTEX_TYPE m_SEEDING_first;
	int m_SEEDING_receivedVertexCoverage;
	bool m_SEEDING_vertexCoverageReceived;
	int m_SEEDING_currentChildRank;
	int m_SEEDING_numberOfIngoingEdgesWithSeedCoverage;
	bool m_SEEDING_vertexCoverageRequested;
	bool m_SEEDING_edge_initiated;
	bool m_SEEDING_NodeInitiated;
	bool m_SEEDING_passedCoverageTest;
	bool m_SEEDING_passedParentsTest;
	bool m_SEEDING_Extended;
	int m_SEEDING_i;
	VERTEX_TYPE m_SEEDING_currentVertex;
	bool m_colorSpaceMode;
	map<int,int> m_FUSION_identifier_map;


	bool m_SEEDING_InedgesReceived;
	bool m_SEEDING_InedgesRequested;
	int m_SEEDING_outgoing_index;
	int m_SEEDING_numberOfSeedCoverageCandidates;
	bool m_SEEDING_outgoing_choice_done;
	bool m_SEEDING_edgesRequested;
	int m_SEEDING_ingoingEdgeIndex;
	int m_SEEDING_outgoingEdgeIndex;
	int m_SEEDING_currentRank;
	vector<vector<VERTEX_TYPE> > m_SEEDING_seeds;
	vector<VERTEX_TYPE> m_SEEDING_seed;
	vector<VERTEX_TYPE> m_SEEDING_receivedIngoingEdges;
	vector<VERTEX_TYPE> m_SEEDING_receivedOutgoingEdges;
	vector<int> m_SEEDING_outgoingCoverages;
	vector<VERTEX_TYPE> m_SEEDING_outgoingKeys;
	bool m_SEEDING_vertexKeyAndCoverageRequested;
	int m_SEEDING_numberOfIngoingEdges;
	set<VERTEX_TYPE> m_SEEDING_vertices;
	int m_SEEDING_numberOfOutgoingEdges;
	bool m_SEEDING_testInitiated;
	bool m_SEEDING_1_1_test_result;
	int m_SEEDING_currentParentRank;
	bool m_SEEDING_1_1_test_done;
	bool m_SEEDING_firstVertexTestDone;
	bool m_SEEDING_firstVertexParentTestDone;	
	bool m_SEEDING_ingoingEdgesDone;
	bool m_SEEDING_outgoingEdgesDone;


	int m_mode_send_coverage_iterator;
	vector<Message> m_outbox;
	vector<Message> m_inbox;


	// EXTENSION MODE
	vector<VERTEX_TYPE> m_enumerateChoices_outgoingEdges;
	bool m_doChoice_tips_Detected;
	PairedRead m_EXTENSION_pairedRead;
	bool m_EXTENSION_pairedSequenceRequested;
	bool m_EXTENSION_hasPairedReadAnswer;
	bool m_EXTENSION_pairedSequenceReceived;
	int m_EXTENSION_edgeIterator;
	bool m_EXTENSION_hasPairedReadRequested;
	bool m_EXTENSION_hasPairedReadReceived;
	vector<int> m_EXTENSION_identifiers;
	set<int> m_FUSION_eliminated;
	vector<VERTEX_TYPE> m_EXTENSION_extension;
	vector<int> m_EXTENSION_coverages;
	bool m_EXTENSION_complementedSeed;
	vector<VERTEX_TYPE> m_EXTENSION_currentSeed;
	int m_EXTENSION_numberOfRanksDone;
	vector<vector<VERTEX_TYPE> > m_EXTENSION_contigs;
	bool m_EXTENSION_checkedIfCurrentVertexIsAssembled;
	bool m_EXTENSION_VertexMarkAssembled_requested;
	bool m_EXTENSION_reverseComplement_requested;
	bool m_EXTENSION_vertexIsAssembledResult;
	set<ReadAnnotation,ReadAnnotationComparator>::iterator m_EXTENSION_readIterator;
	bool m_EXTENSION_readLength_requested;
	bool m_EXTENSION_readLength_received;
	bool m_EXTENSION_readLength_done;
	bool m_EXTENSION_read_vertex_received;
	bool m_EXTENSION_read_vertex_requested;
	VERTEX_TYPE m_EXTENSION_receivedReadVertex;
	bool m_mode_EXTENSION;
	bool m_EXTENSION_currentRankIsDone;
	bool m_EXTENSION_currentRankIsSet;
	bool m_EXTENSION_currentRankIsStarted;
	int m_EXTENSION_rank;
	bool m_EXTENSION_initiated;
	int m_EXTENSION_currentSeedIndex;
	bool m_EXTENSION_VertexAssembled_received;
	int m_EXTENSION_currentPosition;
	bool m_EXTENSION_VertexMarkAssembled_received;
	bool m_EXTENSION_markedCurrentVertexAsAssembled;
	bool m_EXTENSION_enumerateChoices;
	bool m_EXTENSION_choose;
	bool m_EXTENSION_directVertexDone;
	bool m_EXTENSION_VertexAssembled_requested;
	bool m_EXTENSION_receivedAssembled;
	bool m_EXTENSION_reverseComplement_received;
	vector<ReadAnnotation> m_EXTENSION_receivedReads;
	bool m_EXTENSION_reads_requested;
	bool m_EXTENSION_reads_received;
	vector<ReadAnnotation> m_EXTENSION_readsOutOfRange;
	int m_EXTENSION_receivedLength;
	bool m_EXTENSION_reverseVertexDone;
	// reads used so far
	set<int> m_EXTENSION_usedReads;
	// reads to check (the ones "in range")
	set<ReadAnnotation,ReadAnnotationComparator> m_EXTENSION_readsInRange;
	bool m_EXTENSION_singleEndResolution;
	map<int,vector<int> > m_EXTENSION_readPositionsForVertices;
	map<int,vector<int> > m_EXTENSION_pairedReadPositionsForVertices;
	map<int,int> m_EXTENSION_reads_startingPositionOnContig;

	
	// COPY Directions.
	int m_COPY_ranks;

	
	// FUSION
	bool m_fusionStarted;
	bool m_FUSION_direct_fusionDone;
	bool m_FUSION_first_done;
	int m_FUSION_numberOfRanksDone;
	bool m_FUSION_last_done;
	int m_FUSION_path_id;
	int m_FUSION_numberOfPaths;
	bool m_FUSION_paths_requested;
	bool m_FUSION_paths_received;
	vector<Direction> m_FUSION_firstPaths;
	bool m_FUSION_path_received;
	map<int,vector<Direction> > m_FUSION_cachedDirections;
	int m_FUSION_receivedLength;
	bool m_FUSION_reverse_fusionDone;
	vector<Direction> m_FUSION_lastPaths;
	vector<int> m_FUSION_matches;
	bool m_FUSION_matches_done;
	bool m_FUSION_pathLengthReceived;
	bool m_FUSION_matches_length_done;
	int m_FUSION_match_index;
	bool m_FUSION_pathLengthRequested;
	vector<Direction> m_FUSION_receivedPaths;
	bool m_FUSION_path_requested;
	Direction m_FUSION_receivedPath;

	// coverage distribubtion
	map<int,VERTEX_TYPE> m_coverageDistribution;
	int m_minimumCoverage;
	int m_peakCoverage;
	int m_seedCoverage;
	int m_numberOfMachinesDoneSendingCoverage;
	
	string m_VERSION;
	bool m_mode_sendDistribution;

	vector<MPI_Request> m_pendingMpiRequest;
	Parameters m_parameters;
	int m_numberOfMachinesDoneSendingEdges;
	SplayTree<VERTEX_TYPE,Vertex> m_subgraph;

	// SEQUENCE DISTRIBUTION
	bool m_reverseComplementVertex;
	int m_distribution_currentSequenceId;
	int m_distribution_file_id;
	int m_distribution_sequence_id;
	bool m_LOADER_isLeftFile;
	bool m_LOADER_isRightFile;
	int m_LOADER_numberOfSequencesInLeftFile;
	vector<Read*>m_distribution_reads;
	int m_LOADER_averageFragmentLength;
	int m_LOADER_deviation;

	// memory allocators
	// m_outboxAllocator, m_inboxAllocator, and m_distributionAllocator are
	// cleaned everynow and then.
	
	// allocator for outgoing messages
	MyAllocator m_outboxAllocator;
	
	// allocator for ingoing messages
	MyAllocator m_inboxAllocator;
	
	// allocator for persistent data
	MyAllocator m_persistentAllocator;

	// allocator for directions in the de Bruijn graph
	MyAllocator m_directionsAllocator;

	// allocator for distribution of data, not persistent.
	MyAllocator m_distributionAllocator;

	vector<Read*> m_myReads;

	bool m_mode_send_vertices;
	int m_mode_send_vertices_sequence_id;
	int m_mode_send_vertices_sequence_id_position;
	int m_numberOfMachinesDoneSendingVertices;

	vector<vector<VERTEX_TYPE> > m_allPaths;
	bool m_aborted;

	bool m_messageSentForEdgesDistribution;

	// COLLECTING things.
	vector<int> m_identifiers;
	// FINISHING.
	int m_cycleNumber;
	bool m_FINISH_fusionOccured;
	vector<vector<Direction> > m_FINISH_pathsForPosition;
	map<int,int> m_FINISH_pathLengths;
	bool m_checkedValidity;
	int m_FINISH_n;
	int m_DISTRIBUTE_n;
	bool m_isFinalFusion;
	bool m_FINISH_hits_computed;
	int m_FINISH_hit;
	int m_selectedPath;
	int m_selectedPosition;
	#ifdef DEBUG
	set<int> m_collisions;
	#endif
	int m_FINISH_positionStart;
	bool m_FINISH_hasHit;
	vector<vector<VERTEX_TYPE> > m_FINISH_newFusions;
	bool m_FINISH_vertex_received;
	VERTEX_TYPE m_FINISH_received_vertex;
	bool m_nextReductionOccured;
	bool m_cycleStarted;
	bool m_reductionOccured;
	bool m_FINISH_vertex_requested;

	// getPaths
	bool m_Machine_getPaths_INITIALIZED;
	bool m_Machine_getPaths_DONE;
	vector<Direction> m_Machine_getPaths_result;

	#ifdef SHOW_STATISTICS
	map<int,int> m_statistics;
	#endif


	// BUBBLE
	BubbleData*m_bubbleData;

	DepthFirstSearchData*m_dfsData;

	int milliSeconds();
	void enumerateChoices();
	void killRanks();
	void attachReads();
	void printStatus();
	void doChoice();
	void checkIfCurrentVertexIsAssembled();
	void markCurrentVertexAsAssembled();
	int getSize();
	bool isAlive();
	void run();
	void do_1_1_test();
	bool isMaster();
	void receiveMessages();
	void loadSequences();
	void processMessages();
	void processMessage(Message*message);
	void sendMessages();
	void checkRequests();
	void processData();
	int getRank();
	void receiveWelcomeMessage(MPI_Status*status);
	int vertexRank(VERTEX_TYPE a);
	void showProgress();
	void getPaths(VERTEX_TYPE vertex);
	void extendSeeds();
	void finishFusions();
	void makeFusions();
	void depthFirstSearch(VERTEX_TYPE root,VERTEX_TYPE a,int b);
public:
	/*
 * this is the only public bit
 */
	Machine(int argc,char**argv);
	void start();
	~Machine();
};


#endif

