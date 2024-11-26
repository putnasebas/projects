package cz.cvut.fit.tjv.realestates.repository;

import cz.cvut.fit.tjv.realestates.domain.Customer;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface JPACustomerRepository extends JpaRepository<Customer, String> {
}
