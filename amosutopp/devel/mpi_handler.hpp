#ifndef MPI_HANDLER_HPP
#define MPI_HANDLER_HPP

class MPIHandler {
public:
    MPIHandler(int argc, char** argv);
    ~MPIHandler();

    int get_world_size() const;
    int get_world_rank() const;

private:
    int world_size;
    int world_rank;
};

#endif // MPI_HANDLER_HPP