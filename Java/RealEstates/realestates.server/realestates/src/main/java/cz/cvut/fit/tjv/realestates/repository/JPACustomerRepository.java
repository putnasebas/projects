package cz.cvut.fit.tjv.realestates.repository;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.time.LocalDate;
import java.util.List;

@Repository
public interface JPACustomerRepository extends JpaRepository<Customer, String> {
    @Query("select C from Customer C where C.birthDate > :birthDate")
    List<Customer> getCustomersBornAfter(@Param("birthDate") LocalDate date);
    @Modifying
    @Query("delete from Customer C where C.birthDate > :birthDate")
    void deleteCustomersBornAfter(@Param("birthDate") LocalDate date);
}
