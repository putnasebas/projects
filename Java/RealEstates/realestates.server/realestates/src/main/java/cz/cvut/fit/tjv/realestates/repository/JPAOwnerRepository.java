package cz.cvut.fit.tjv.realestates.repository;

import cz.cvut.fit.tjv.realestates.domain.Owner;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface JPAOwnerRepository extends JpaRepository<Owner, String> { }
