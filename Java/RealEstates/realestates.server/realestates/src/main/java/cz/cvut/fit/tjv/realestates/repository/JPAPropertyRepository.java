package cz.cvut.fit.tjv.realestates.repository;

import cz.cvut.fit.tjv.realestates.domain.Owner;
import cz.cvut.fit.tjv.realestates.domain.Property;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface JPAPropertyRepository extends JpaRepository<Property, Long> {
    @Query(value = "SELECT P from Property P where :firstOwner member of P.owners and :secondOwner member of P.owners")
    List<Property> findOwnerIntersection(@Param("firstOwner") Owner firstOwner, @Param("secondOwner") Owner secondOwner);
}
